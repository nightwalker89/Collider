## GPU BSGS Collider v1.7.7
#### Cuda (Nvidia card only) and Windows x64
![alt text](x64/large-bitcoin-collider.png "Collider")<br />
Forked from [Etayson/BSGS-cuda](https://github.com/Etayson/BSGS-cuda)<br />
## Help page: Collider.exe -h
```
C:\Users\User>Collider.exe -h

-t       Number of GPU threads, Default 512
-b       Number of GPU blocks, Default 68
-p       Number of pparam, Default 256
-d       Select GPU IDs, Default 0 (-d 1,2,3)
-pb      Set single uncompressed/compressed pubkey for searching
-pk      Range start from , Default 0x1
-pke     End range
-w       Set number of baby items 2^ (-w 22  mean 2^22 points)
-htsz    Set number of HashTable 2^ , Default 26
-infile  Set file with pubkey for searching in uncompressed/compressed  format (search sequential), one pubkey per line
-wl      Set recovery file from which the state will be loaded
-wt      Set timer for autosaving current state, Default every 180 seconds
```
### Collider Speed: 
- (5) RTX 3080 (10GB) = ~10 Ekeys/s
- RTX 3090 (24GB)     = ~62bit/s
- RTX 2070 (8 GB)     = ~1,6 Ekeys/s (1,652,921,504,606,846,975 private keys/s)

### Search [Puzzles 120-160](https://privatekeys.pw/puzzles/bitcoin-puzzle-tx)<br />
#120 17s2b9ksz5y7abUm92cHwG8jEPCzK3dLnT<br />
-pk 800000000000000000000000000000<br />
-pke FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630<br /><br />
#125 1PXAyUB8ZoH3WD8n5zoAthYjN15yN5CVq5<br />
-pk 10000000000000000000000000000000<br />
-pke 1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 0233709EB11E0D4439A729F21C2C443DEDB727528229713F0065721BA8FA46F00E<br /><br />
#130 1Fo65aKq8s8iquMt6weF1rku1moWVEd5Ua <br />
-pk 200000000000000000000000000000000<br />
-pke 3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 03633CBE3EC02B9401C5EFFA144C5B4D22F87940259634858FC7E59B1C09937852<br /><br />
#135 16RGFo6hjq9ym6Pj7N5H7L1NR1rVPJyw2v<br />
-pk 4000000000000000000000000000000000<br />
-pke 7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 02145D2611C823A396EF6712CE0F712F09B9B4F3135E3E0AA3230FB9B6D08D1E16<br /><br />
#140 1QKBaU6WAeycb3DbKbLBkX7vJiaS8r42Xo<br />
-pk 80000000000000000000000000000000000<br />
-pke FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 031F6A332D3C5C4F2DE2378C012F429CD109BA07D69690C6C701B6BB87860D6640<br /><br />
#145 19GpszRNUej5yYqxXoLnbZWKew3KdVLkXg<br />
-pk 1000000000000000000000000000000000000<br />
-pke 1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 03AFDDA497369E219A2C1C369954A930E4D3740968E5E4352475BCFFCE3140DAE5<br /><br />
#150 1MUJSJYtGPVGkBCTqGspnxyHahpt5Te8jy<br />
-pk 20000000000000000000000000000000000000<br />
-pke 3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 03137807790EA7DC6E97901C2BC87411F45ED74A5629315C4E4B03A0A102250C49<br /><br />
#155 1AoeP37TmHdFh8uN72fu9AqgtLrUwcv2wJ<br />
-pk 400000000000000000000000000000000000000<br />
-pke 7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 035CD1854CAE45391CA4EC428CC7E6C7D9984424B954209A8EEA197B9E364C05F6<br /><br /><br />
#160 1NBC8uXJy1GiJ6drkiZa1WuKn51ps7EPTv<br />
-pk 8000000000000000000000000000000000000000<br />
-pke FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF<br />
-pb 02E0A8B039282FAF6FE0FD769CFBC4B6B4CF8758BA68220EAC420E32B91DDFA673<br />

## Use:
Current state is always saved to file currentwork.txt <br />
If app crash or you stop app, you can start working from the last saved state. Provided the launch configuration has not been changed. <br />
Note! set minimal -htsz value depending on -w <br />

|  Value     |  GPU Memory |
| ---------- | ----------- |  
|   -w 30    |  11.03 GB   |
|   -w 29    |   8.01 GB   |
|   -w 28    |   7.01 GB   |
|   -w 27    |   6.02 GB   |

|   Value    |     RAM     |
| ---------- | ----------- |
|   -w 30    |   -htsz 28  32GB|
|   -w 29    |   -htsz 28  |
|   -w 28    |   -htsz 27  |
|   -w 27    |   -htsz 25  |

All arrays(Baby, Giant) and hashtable saved to the disk for fast spinup solver next time (if parameters will not changed). <br />
After you have the arrays saved, you will need less RAM to launch. <br />

Example range 64 bit:<br />
Run test: ```Collider.exe -t 512 -b 72 -p 306 -pk 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000 -pke 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff -w 30 -htsz 28 -pb 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19```<br />

## Example work:
```
C:\Users\User>Collider.exe -t 512 -b 72 -p 306 -pk 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000 -pke 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff -w 30 -htsz 28 -pb 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19
  Collider      : v1.7.7
  GPU threads   : 512
  GPU blocks    : 72
  Pparam        : 306
  Items number set to 2^30
  HT size number set to 2^28
  Pubkey set to 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19
  USE GPUs      : 1
  GPU           : NVIDIA GeForce RTX 2070 (7181/8191 MB)
  GPU have      : MP: 36 and 2304 cores
  Try parameters: -t 512 -b 72 -p 306 -w 30 -htsz 28 [7176.750 MB] Gen RAM[28672 MB]
  Current config: hash[7ecfaf7788a641351af7d8ee7e1b37bdcd8a5bbb]
  ***********************************
  Total GPU Memory Need: 7176.750Mb
  ***********************************
  Both HT files exist
  Load BIN file:79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798_1073741824_268435456_htGPU.BIN
  Generate Giants Buffer: 11280384 items
  Load BIN file: 512_72_306_1073741824_g2.BIN
  Done in 00:00:00s
  GPU count #1
  GPU #0 launched
  GPU #0 Free memory  : 7179 MB
  GPU #0 Total memory : 8191 MB
  GPU #0 TotalBuff    : 7176.750 MB
  Load BIN file: 79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798_1073741824_268435456_htCPU.BIN
  Global start : 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000
  Global end   : 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff
  Global range : 000000000000000000000000000000000000000000000000ffffffffffffffff
  Checkpoint   : Save every : 180 seconds
  Site         : https://github.com/phrutis/Collider
  Donate       : bc1qh2mvnf5fujg93mwl8pe688yucaw9sflmwsukz9
  FINDpubkey   : 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19

  [00:00:12] [f008a00000000001] [F: 1] [GPU: 661 Mk/s] [BSGS: 60.37 bit]
  ================================== FOUND (1) ===================================
  Privat key   : 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5eff7777f123456789
  Public key   : 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19
  ================================================================================
  Work time    : 00:00:12
  Total time   : 00:00:21
  GPU          : 0 job finished
  GPU          : 0 thread finished
  Cuda finished: ok

  Press Enter to exit
```
- Solved Private keys will be saved to the **Found.txt** file!

## Random mode:
- For random mode, you need to create a Collider-Random.bat file 
- The parameters for creating a baht file must correspond to the parameters of the saved table. 
- If they do not match, instead of random, new bin tables will be created, and the timer will reload the Collider.

### Help page Random.exe -h
```
C:\Users\User>Random.exe -h
  Random for Collider    [OPTIONS...]
-h, --help             : Display this message
-t, --t                : Number of GPU threads, Default -t 512
-b, --b                : Number of GPU blocks,  Default -b 68
-p, --p                : Number of pparam,      Default -p 256

-pb, --pb              : Set single Public key (uncompressed or compressed)
                       : Default (puzzle120): -pb 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630

--range                : START:END Random between. Example --range 1111111:7777777
                       : For puzzle120: --range 800000000000000000000000000001:ffffffffffffffffffffffffffffff

-w, --w                : Set number of baby items 2^ (-w 22  mean 2^22 points) Default -w 22
-htsz, --htsz          : Set number of HashTable 2^ , Default -htsz 26
-time, --time          : Time in seconds after how many to update the private key. Default -time 300
-n, --n                : How many random start private keys to generate? Default -n 1000

Example: Random.exe -t 512 -b 72 -p 306 --range 800000000000000000000000000001:ffffffffffffffffffffffffffffff -w 30 -htsz 28 -pb 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630 -time 300 -n 1000 
```
### Example of creating a bat file :
```
Random.exe -t 512 -b 72 -p 306 --range 800000000000000000000000000001:ffffffffffffffffffffffffffffff -w 30 -htsz 28 -pb 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630 -time 300 -n 1000 

 Random for Collider v1.0 (27.11.2021)

[+] Your Random setting for Collider
[+] Teheads    -t      : 512
[+] Blocks     -b      : 72
[+] Points     -p      : 306
[+] Stat range -pk     : (120 bit) 800000000000000000000000000001
[+] End range  -pe     : (120 bit) FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
[+] Parameter  -w      : 30
[+] Parameter  -htsz   : 28
[+] Public key -pb     : 02CEB6CBBCDBDF5EF7150682150F4CE2C6F4807B349827DCDBDD1F2EFA885A2630
[+] Reload every -time : 300 sec. (5 min)
[+] Random cycles -n   : 1000

[>] Site: https://github.com/phrutis/LostCoins
[>] Donate: bc1qh2mvnf5fujg93mwl8pe688yucaw9sflmwsukz9

  Bat file for randomness is created!
```
- For start Run: ```Collider-Random.bat```

## Building
- To compile the Cpllider you need [Purebasic v5.31](https://www.purebasic.com)

## Donation
- BTC: bc1qh2mvnf5fujg93mwl8pe688yucaw9sflmwsukz9

## __Disclaimer__
ALL THE CODES, PROGRAM AND INFORMATION ARE FOR EDUCATIONAL PURPOSES ONLY. USE IT AT YOUR OWN RISK. THE DEVELOPER WILL NOT BE RESPONSIBLE FOR ANY LOSS, DAMAGE OR CLAIM ARISING FROM USING THIS PROGRAM.
