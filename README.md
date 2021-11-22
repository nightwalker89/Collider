# GPU BSGS Collider
#### Cuda (Nvidia card only) and Windows x64
![alt text](x64/large-bitcoin-collider.png "Collider")<br />
Forked from [Etayson/BSGS-cuda](https://github.com/Etayson/BSGS-cuda)<br />
### Help page: Collider.exe -h
```
C:\Users\User>Collider.exe -h

-t       Number of GPU threads, default 512
-b       Number of GPU blocks, default 68
-p       Number of pparam, default 256
-d       Select GPU IDs, default 0 (-d 1,2,3)
-pb      Set single uncompressed/compressed pubkey for searching
-pk      Range start from , default 0x1
-pke     End range
-w       Set number of baby items 2^ (-w 22  mean 2^22 points)
-htsz    Set number of HashTable 2^ , default 26
-infile  Set file with pubkey for searching in uncompressed/compressed  format (search sequential), one pubkey per line
-wl      Set recovery file from which the state will be loaded
-wt      Set timer for autosaving current state, default every 180seconds
```
### Collider speed (32GB RAM): 
- RTX 3090 (24GB) = ~62bit/s or ~2^62.15/s
- RTX 2070 (8 GB) = ~60bit/s or ~1,1Ekeys/s 
(1,152,921,504,606,846,975 private keys/s)

### Use:
Current state is always saved to file currentwork.txt <br />
If app crash or you stop app, you can start working from the last saved state. Provided the launch configuration has not been changed. <br />
Note! set minimal -htsz value depending on -w <br />

|  value     |  GPU Memory |
| ---------- | ----------- |
|   -w 30    |  11.03 GB   |
|   -w 29    |   8.01 GB   |
|   -w 28    |   7.07 GB   |
|   -w 27    |   6.02 GB   |

|   value    |     RAM     |
| ---------- | ----------- |
|   -w 30    |   -htsz 28  32GB|
|   -w 29    |   -htsz 28  |
|   -w 28    |   -htsz 27  |
|   -w 27    |   -htsz 25  |

All arrays(Baby, Giant) and hashtable saved to the disk for fast spinup solver next time (if parameters will not changed). <br />
After you have the arrays saved, you will need less RAM to launch. <br />

Example range 64 bit:<br />
In 1.txt Publik key: 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19<br /><br />
Run test: ```Collider.exe -t 512 -b 72 -p 306 -pk 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000 -pke 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff -w 30 -htsz 28 -infile 1.txt```<br />

### Example work:
```
C:\Users\User>Collider.exe -t 512 -b 72 -p 306 -pk 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000 -pke 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff -w 30 -htsz 28 -infile 1.txt
Number of GPU threads set to #512
Number of GPU blocks set to #72
Number of pparam set to #306
Range begin: 0x49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000
Range end: 0x49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff
Items number set to 2^30
HT size number set to 2^28
Will be used file: 1.txt
APP VERSION: 1.7.3
Found 1 Cuda device.
Cuda device:NVIDIA GeForce RTX 2070 (7181.000/8191MB)
Device have: MP:36 Cores+2304
Try -t 512 -b 72 -p 306 -w 30 -htsz 28 [7176.750 MB] Gen RAM[28672 MB]
---------------
Current config hash[7ecfaf7788a641351af7d8ee7e1b37bdcd8a5bbb]
GiantSUBvalue:0000000000000000000000000000000000000000000000000000000080000000
GiantSUBpubkey: 025318f9b1a2697010c5ac235e9af475a8c7e5419f33d47b18d33feeb329eb99a4
*******************************
Total GPU Memory Need: 7176.750Mb
*******************************
Both HT files exist
Load BIN file:79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798_1073741824_268435456_htGPU.BIN
[0] chunk:1073741824b
[1] chunk:1073741824b
[2] chunk:1073741824b
[3] chunk:1073741824b
[4] chunk:1073741824b
[5] chunk:1073741824b
Generate Giants Buffer: 11280384 items
Giant #0  (5318f9b1a2697010c5ac235e9af475a8c7e5419f33d47b18d33feeb329eb99a40bb33014b4125be6a88d26c5144bfa175be0d4bf2e1c139ad38d9110016e0302)
Verify giant array...ok
Prepear Giant Buffer for GPU using...ok
Convert BigIntegers to 32b...ok
Freed memory: 688.500 MB
Save BIN file:512_72_306_1073741824_g2.BIN
[0] chunk:721944576b
Saved:721944576 bytes
Done in 00:00:37s
GPU count #1
GPU #0 launched
GPU #0 Free memory: 7179Mb
GPU #0 Total memory: 8191Mb
GPU #0 TotalBuff: 7176.750Mb
Load BIN file:79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798_1073741824_268435456_htCPU.BIN
[0] chunk:1073741824b
[1] chunk:1073741824b
[2] chunk:1073741824b
[3] chunk:1073741824b
[4] chunk:1073741824b
[5] chunk:1073741824b
[6] chunk:1073741824b
[7] chunk:1073741824b
[8] chunk:1073741824b
[9] chunk:1073741824b
START RANGE= 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5e0000000000000000
  END RANGE= 49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5effffffffffffffff
WIDTH RANGE= 000000000000000000000000000000000000000000000000ffffffffffffffff
SUBpoint= (3c52f78892c8c2f5c51d7249951bbb1c302a8ed4d37561724e68e8d22db14a69, e0ba5063f64117bccd7fc6c1d5b97df4f0bdc5a6ba481f21e69da330ed9750ae)
Save work every 180 seconds

FINDpubkey: 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19
Cnt:ebffe00000000001 [1][ 652 ] = 652 MKeys/s x2^31=2^60.35 t:00:00:12
KEY[1]: 0x49dccfd96dc5df56487436f5a1b18c4f5d34f65ddb48cb5eff7777f123456789
   Pub: 03c5bcdd76b64cbbd8212080fe5efa9bf577cdcaac9f5853b216e71723ec3aca19
Working time 00:00:13s
Total time 00:01:00s
GPU#0 job finished
GPU#0 thread finished
cuda finished ok
```
Solved pubkeys will be saved to the **win.txt** file!

## Compilation
- To compile the Cpllider you need [**Purebasic v5.31**](https://www.purebasic.com)

