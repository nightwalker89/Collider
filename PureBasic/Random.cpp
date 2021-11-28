#include "Timer.h"
#include "Rotor.h"
#include "Base58.h"
#include "CmdParse.h"
#include <fstream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <windows.h>
#ifndef WIN64
#include <signal.h>
#include <unistd.h>
#endif

#define RELEASE "1.0 (27.11.2021)"

using namespace std;
bool should_exit = false;

// ----------------------------------------------------------------------------
void usage()
{
	printf("  Random for Collider    [OPTIONS...]\n");
	printf("-h, --help             : Display this message\n");
	printf("-t, --t                : Number of GPU threads, Default -t 512\n");
	printf("-b, --b                : Number of GPU blocks,  Default -b 68\n");
	printf("-p, --p                : Number of pparam,      Default -p 256\n\n");
	printf("-pb, --pb              : Set single Public key (uncompressed or compressed) \n");
	printf("                       : Default (puzzle120): -pb 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630\n\n");
	printf("--range                : START:END Random between. Example --range 1111111:7777777 \n");
	printf("                       : For puzzle120: --range 800000000000000000000000000001:ffffffffffffffffffffffffffffff \n\n");
	printf("-w, --w                : Set number of baby items 2^ (-w 22  mean 2^22 points) Default -w 22\n");
	printf("-htsz, --htsz          : Set number of HashTable 2^ , Default -htsz 26\n");
	printf("-time, --time          : Time in seconds after how many to update the private key. Default -time 300\n");
	printf("-n, --n                : How many random start private keys to generate? Default -n 1000\n\n");
	printf("Example: Random.exe -t 512 -b 72 -p 306 --range 800000000000000000000000000001:ffffffffffffffffffffffffffffff -w 30 -htsz 28 -pb 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630 -time 300 -n 1000\n");

}

// ----------------------------------------------------------------------------

void getInts(string name, vector<int>& tokens, const string& text, char sep)
{

	size_t start = 0, end = 0;
	tokens.clear();
	int item;

	try {

		while ((end = text.find(sep, start)) != string::npos) {
			item = std::stoi(text.substr(start, end - start));
			tokens.push_back(item);
			start = end + 1;
		}

		item = std::stoi(text.substr(start));
		tokens.push_back(item);

	}
	catch (std::invalid_argument&) {

		printf("  Invalid %s argument, number expected\n", name.c_str());
		usage();
		exit(-1);

	}

}

bool parseRange(const std::string& s, Int& start, Int& end)
{
	size_t pos = s.find(':');

	if (pos == std::string::npos) {
		start.SetBase16(s.c_str());
		end.Set(&start);
		end.Add(0xFFFFFFFFFFFFULL);
	}
	else {
		std::string left = s.substr(0, pos);

		if (left.length() == 0) {
			start.SetInt32(1);
		}
		else {
			start.SetBase16(left.c_str());
		}

		std::string right = s.substr(pos + 1);

		if (right[0] == '+') {
			Int t;
			t.SetBase16(right.substr(1).c_str());
			end.Set(&start);
			end.Add(&t);
		}
		else {
			end.SetBase16(right.c_str());
		}
	}

	return true;
}

#ifdef WIN64
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
		//printf("\n\nCtrl-C event\n\n");
		should_exit = true;
		return TRUE;

	default:
		return TRUE;
	}
}
#else
void CtrlHandler(int signum) {
	printf("\n\n  BYE\n");
	exit(signum);
}
#endif

int main(int argc, char** argv)
{
	

	// Global Init
	Timer::Init();
	rseed(Timer::getSeed32());
	struct console
	{
		console(unsigned width, unsigned height)
		{
			SMALL_RECT r;
			COORD      c;
			hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (!GetConsoleScreenBufferInfo(hConOut, &csbi))
				throw runtime_error("You must be attached to a human.");

			r.Left =
				r.Top = 0;
			r.Right = width - 1;
			r.Bottom = height - 1;
			SetConsoleWindowInfo(hConOut, TRUE, &r);

			c.X = width;
			c.Y = height;
			SetConsoleScreenBufferSize(hConOut, c);
		}

		~console()
		{
			SetConsoleTextAttribute(hConOut, csbi.wAttributes);
			SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
			SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
		}

		void color(WORD color = 0x07)
		{
			SetConsoleTextAttribute(hConOut, color);
		}

		HANDLE                     hConOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
	};

	//----------------------------------------------------------------------------
	console con(220, 1000);


	bool gpuEnable = false;
	bool gpuAutoGrid = true;
	int compMode = SEARCH_COMPRESSED;
	vector<int> gpuId = { 0 };
	vector<int> gridSize;
	int nbit2 = 0;
	int display = 1;
	int zet = 0;
	string outputFile = "Found.txt";

	string inputFile = "";	// for both multiple hash160s and x points
	string address = "";	// for single address mode
	string xpoint = "";		// for single x point mode

	std::vector<unsigned char> hashORxpoint;
	bool singleAddress = false;
	int nbCPUThread = Timer::getCoreNumber();

	bool tSpecified = false;
	bool useSSE = true;
	uint32_t maxFound = 1024 * 64;
	int color = 14;
	int color2 = 15;
	int color3 = 10;
	int color4 = 11;
	int color5 = 12;
	uint64_t rKey = 0;
	int next = 0;
	Int rangeStart;
	Int rangeEnd;
	rangeStart.SetInt32(0);
	rangeEnd.SetInt32(0);

	int searchMode = 0;
	int coinType = COIN_BTC;
	int b = 68;
	int t = 512;
	int p = 256;
	int n = 1000;

	string pk = "800000000000000000000000000001";
	string pke = "ffffffffffffffffffffffffffffff";
	string pb = "02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630";

	int w = 22;
	int htsz = 26;
	int time = 300;
	int d = 0;

	hashORxpoint.clear();

	// cmd args parsing
	CmdParse parser;
	parser.add("-h", "--help", false);
	parser.add("-c", "--check", false);
	parser.add("-l", "--list", false);
	parser.add("-v", "--version", false);

	parser.add("-b", "--b", true);
	parser.add("-t", "--t", true);
	parser.add("-p", "--p", true);
	parser.add("-n", "--n", true);

	parser.add("", "--range", true);
	parser.add("-pb", "--pb", true);

	parser.add("-w", "--w", true);
	parser.add("-htsz", "--htsz", true);
	parser.add("-time", "--time", true);
	parser.add("-d", "--d", true);


	if (argc == 1) {
		usage();
		return 0;
	}
	try {
		parser.parse(argc, argv);
	}
	catch (std::string err) {
		printf("  Error: %s\n", err.c_str());
		usage();
		exit(-1);
	}
	std::vector<OptArg> args = parser.getArgs();

	for (unsigned int i = 0; i < args.size(); i++) {
		OptArg optArg = args[i];
		std::string opt = args[i].option;

		try {
			if (optArg.equals("-h", "--help")) {
				usage();
				return 0;
			}
			else if (optArg.equals("-c", "--check")) {
				printf("  Collider v" RELEASE "\n\n");
				printf("\n  Checking... Secp256K1\n\n");
				Secp256K1* secp = new Secp256K1();
				secp->Init();
				secp->Check();
				printf("\n\n  Checking... Int\n\n");
				Int* K = new Int();
				K->SetBase16("3EF7CEF65557B61DC4FF2313D0049C584017659A32B002C105D04A19DA52CB47");
				K->Check();
				delete secp;
				delete K;
				printf("\n\n  Checked successfully\n\n");
				return 0;
			}
			else if (optArg.equals("-l", "--list")) {
#ifdef WIN64
				GPUEngine::PrintCudaInfo();
#else
				printf("  GPU code not compiled, use -DWITHGPU when compiling.\n");
#endif
				return 0;
			}
			else if (optArg.equals("-b", "--b")) {
				b = std::stoi(optArg.arg);
			}
			else if (optArg.equals("-t", "--t")) {
				t = std::stoi(optArg.arg);
			}
			else if (optArg.equals("-p", "--p")) {
				p = std::stoi(optArg.arg);
			}
			else if (optArg.equals("-n", "--n")) {
				n = std::stoi(optArg.arg);
			}
			else if (optArg.equals("", "--range")) {
				std::string range = optArg.arg;
				parseRange(range, rangeStart, rangeEnd);
			}
			else if (optArg.equals("-pb", "--pb")) {
				pb = optArg.arg;
			}
			else if (optArg.equals("-w", "--w")) {
				w = std::stoi(optArg.arg);
			}
			else if (optArg.equals("-htsz", "--htsz")) {
				htsz = std::stoi(optArg.arg);
			}
			else if (optArg.equals("-time", "--time")) {
				time = std::stoi(optArg.arg);
			}
			else if (optArg.equals("-d", "--d")) {
				d = std::stoi(optArg.arg);
			}
		}
		catch (std::string err) {
			printf("Error: %s\n", err.c_str());
			usage();
			return -1;
		}
	}
	
	// Parse operands
	std::vector<std::string> ops = parser.getOperands();
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);

	printf("\n");
	printf(" Random for Collider v" RELEASE "\n");
	printf("\n");

	FILE* ptrFile7 = fopen("Collider-Random.bat", "w+");

	fprintf(ptrFile7, "@echo off \n");
	fprintf(ptrFile7, "cd %%~dp0 \n");
	fprintf(ptrFile7, "COLOR E \n");
	fprintf(ptrFile7, ":start \n");

	for (int i = 1; i < n; i++) {
		Int spirt;
		spirt.Rand2(&rangeStart, &rangeEnd);
		fprintf(ptrFile7, "start /min Collider.exe -t %d -b %d -p %d -pk %s -pke %s -w %d -htsz %d -wt 77777 -pb %s\n", t, b, p, spirt.GetBase16().c_str(), rangeEnd.GetBase16().c_str(), w, htsz, pb.c_str());
		fprintf(ptrFile7, "timeout /t %d /nobreak \n", time);
		fprintf(ptrFile7, "taskkill /f /im Collider.exe\n");
		fprintf(ptrFile7, "goto :next-%d\n", i);
		fprintf(ptrFile7, ":next-%d \n", i);
	}
	fprintf(ptrFile7, "goto :start \n");
	fclose(ptrFile7);

	SetConsoleTextAttribute(hConsole, color2);

	printf("[+] Your Random setting for Collider\n");
	printf("[+] Teheads    -t      : %d\n", t);
	printf("[+] Blocks     -b      : %d\n", b);
	printf("[+] Points     -p      : %d\n", p);
	printf("[+] Stat range -pk     : (%d bit) %s\n", rangeStart.GetBitLength(), rangeStart.GetBase16().c_str());
	printf("[+] End range  -pe     : (%d bit) %s\n", rangeEnd.GetBitLength(), rangeEnd.GetBase16().c_str());
	printf("[+] Parameter  -w      : %d\n", w);
	printf("[+] Parameter  -htsz   : %d\n", htsz);
	printf("[+] Public key -pb     : %s\n", pb.c_str());
	
	if (time < 60) {
		SetConsoleTextAttribute(hConsole, color5);
		printf("[+] Reload every -time : %d sec. Refresh every %d sec. this is bad! I recommend -n 300 (5 min.) or -n 600 (10 min.)\n", time, time);
	}
	else {
		int mn;
		mn = time / 60;
		printf("[+] Reload every -time : %d sec. (%d min)\n", time, mn);
	}

	printf("[+] Random cycles -n   : %d\n\n", n);
	
	SetConsoleTextAttribute(hConsole, color4);
	printf("[>] Site: https://github.com/phrutis/LostCoins \n");
	printf("[>] Donate: bc1qh2mvnf5fujg93mwl8pe688yucaw9sflmwsukz9 \n\n");
	SetConsoleTextAttribute(hConsole, color3);
	printf("  Bat file for randomness is created! \n\n");
	exit(-1);

}