/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "syscalls.h"

syscallsMeta sysCalls[2][312];

void initSysCalls () {
	//sysCalls[i363][0][1].name = "exit";
	sysCalls[i363][1].types[0] = scInt;
	sysCalls[i363][1].types[1] = scNone;
	sysCalls[i363][1].types[2] = scNone;
	sysCalls[i363][1].types[3] = scNone;
	sysCalls[i363][1].types[4] = scNone;
	sysCalls[i363][1].res = scInt;

	//sysCalls[i363][3].name = "read";
	sysCalls[i363][3].types[0] = scUIntFD;
	sysCalls[i363][3].types[1] = scCharRefWithSize;
	sysCalls[i363][3].types[2] = scSizeT;
	sysCalls[i363][3].types[3] = scNone;
	sysCalls[i363][3].types[4] = scNone;
	sysCalls[i363][3].res = scInt;

	//sysCalls[i363][4].name = "write";
	sysCalls[i363][4].types[0] = scUIntFD;
	sysCalls[i363][4].types[1] = scCharRefWithSize;
	sysCalls[i363][4].types[2] = scSizeT;
	sysCalls[i363][4].types[3] = scNone;
	sysCalls[i363][4].types[4] = scNone;
	sysCalls[i363][4].res = scInt;

	//sysCalls[i363][5].name = "open";
	sysCalls[i363][5].types[0] = scConstCharRef;
	sysCalls[i363][5].types[1] = scInt;
	sysCalls[i363][5].types[2] = scInt;
	sysCalls[i363][5].types[3] = scNone;
	sysCalls[i363][5].types[4] = scNone;
	sysCalls[i363][5].res = scInt;

	//sysCalls[i363][6].name = "close";
	sysCalls[i363][6].types[0] = scUIntFD;
	sysCalls[i363][6].types[1] = scNone;
	sysCalls[i363][6].types[2] = scNone;
	sysCalls[i363][6].types[3] = scNone;
	sysCalls[i363][6].types[4] = scNone;
	sysCalls[i363][6].res = scInt;

	//sysCalls[i363][7].name = "waitpid";
	sysCalls[i363][7].types[0] = scPIDt;
	sysCalls[i363][7].types[1] = scIntRef;
	sysCalls[i363][7].types[2] = scInt;
	sysCalls[i363][7].types[3] = scNone;
	sysCalls[i363][7].types[4] = scNone;
	sysCalls[i363][7].res = scInt;

	//sysCalls[i363][8].name = "creat";
	sysCalls[i363][8].types[0] = scConstCharRef;
	sysCalls[i363][8].types[1] = scInt;
	sysCalls[i363][8].types[2] = scNone;
	sysCalls[i363][8].types[3] = scNone;
	sysCalls[i363][8].types[4] = scNone;
	sysCalls[i363][8].res = scInt;

	//sysCalls[i363][8].name = "chdir";
	sysCalls[i363][8].types[0] = scConstCharRef;
	sysCalls[i363][8].types[1] = scNone;
	sysCalls[i363][8].types[2] = scNone;
	sysCalls[i363][8].types[3] = scNone;
	sysCalls[i363][8].types[4] = scNone;
	sysCalls[i363][8].res = scInt;

	//sysCalls[i363][11].name = "execve";
	sysCalls[i363][11].types[0] = scConstCharRef;
	sysCalls[i363][11].types[1] = scConstCharRef;
	sysCalls[i363][11].types[2] = scConstCharRef;
	sysCalls[i363][11].types[3] = scNone;
	sysCalls[i363][11].types[4] = scNone;
	sysCalls[i363][11].res = scInt;

	//sysCalls[i363][12].name = "chdir";
	sysCalls[i363][12].types[0] = scConstCharRef;
	sysCalls[i363][12].types[1] = scNone;
	sysCalls[i363][12].types[2] = scNone;
	sysCalls[i363][12].types[3] = scNone;
	sysCalls[i363][12].types[4] = scNone;
	sysCalls[i363][12].res = scInt;


	//sysCalls[i363][13].name = "time";
	sysCalls[i363][13].types[0] = scNone;
	sysCalls[i363][13].types[1] = scNone;
	sysCalls[i363][13].types[2] = scNone;
	sysCalls[i363][13].types[3] = scNone;
	sysCalls[i363][13].types[4] = scNone;
	sysCalls[i363][13].res = scInt;

	//sysCalls[i363][19].name = "lseek";
	sysCalls[i363][19].types[0] = scUIntFD;
	sysCalls[i363][19].types[1] = scOFFT;
	sysCalls[i363][19].types[2] = scInt;
	sysCalls[i363][19].types[3] = scNone;
	sysCalls[i363][19].types[4] = scNone;
	sysCalls[i363][19].res = scOFFT;

	//sysCalls[i363][21].name = "mount";
	sysCalls[i363][21].types[0] = scConstCharRef;
	sysCalls[i363][21].types[1] = scConstCharRef;
	sysCalls[i363][21].types[2] = scConstCharRef;
	sysCalls[i363][21].types[3] = scULong;
	sysCalls[i363][21].types[4] = scNone;
	sysCalls[i363][21].res = scInt;


	//sysCalls[i363][33].name = "access";
	sysCalls[i363][33].types[0] = scConstCharRef;
	sysCalls[i363][33].types[1] = scInt;
	sysCalls[i363][33].types[2] = scNone;
	sysCalls[i363][33].types[3] = scNone;
	sysCalls[i363][33].types[4] = scNone;
	sysCalls[i363][33].res = scInt;

	//sysCalls[i363][43].name = "times";
	sysCalls[i363][43].types[0] = scNone;
	sysCalls[i363][43].types[1] = scNone;
	sysCalls[i363][43].types[2] = scNone;
	sysCalls[i363][43].types[3] = scNone;
	sysCalls[i363][43].types[4] = scNone;
	sysCalls[i363][43].res = scInt;


	//sysCalls[i363][45].name = "brk";
	sysCalls[i363][45].types[0] = scNone;
	sysCalls[i363][45].types[1] = scNone;
	sysCalls[i363][45].types[2] = scNone;
	sysCalls[i363][45].types[3] = scNone;
	sysCalls[i363][45].types[4] = scNone;
	sysCalls[i363][45].res = scInt;


	//sysCalls[i363][91].name = "munmap";
	sysCalls[i363][91].types[0] = scNone;
	sysCalls[i363][91].types[1] = scNone;
	sysCalls[i363][91].types[2] = scNone;
	sysCalls[i363][91].types[3] = scNone;
	sysCalls[i363][91].types[4] = scNone;
	sysCalls[i363][91].res = scInt;

	//sysCalls[i363][92].name = "truncate";
	sysCalls[i363][92].types[0] = scConstCharRef;
	sysCalls[i363][92].types[1] = scULong;
	sysCalls[i363][92].types[2] = scNone;
	sysCalls[i363][92].types[3] = scNone;
	sysCalls[i363][92].types[4] = scNone;
	sysCalls[i363][92].res = scInt;

	//sysCalls[i363][93].name = "ftruncate";
	sysCalls[i363][93].types[0] = scUIntFD;
	sysCalls[i363][93].types[1] = scULong;
	sysCalls[i363][93].types[2] = scNone;
	sysCalls[i363][93].types[3] = scNone;
	sysCalls[i363][93].types[4] = scNone;
	sysCalls[i363][93].res = scInt;

	//sysCalls[i363][96].name = "getpriority";
	sysCalls[i363][96].types[0] = scInt;
	sysCalls[i363][96].types[1] = scInt;
	sysCalls[i363][96].types[2] = scNone;
	sysCalls[i363][96].types[3] = scNone;
	sysCalls[i363][96].types[4] = scNone;
	sysCalls[i363][96].res = scInt;

	//sysCalls[i363][97].name = "setpriority";
	sysCalls[i363][97].types[0] = scInt;
	sysCalls[i363][97].types[1] = scInt;
	sysCalls[i363][97].types[2] = scInt;
	sysCalls[i363][97].types[3] = scNone;
	sysCalls[i363][97].types[4] = scNone;
	sysCalls[i363][97].res = scInt;

	//sysCalls[i363][102].name = "socketcall";
	sysCalls[i363][102].types[0] = scUInt;
	sysCalls[i363][102].types[1] = scULongSocketRef;
	sysCalls[i363][102].types[2] = scNone;
	sysCalls[i363][102].types[3] = scNone;
	sysCalls[i363][102].types[4] = scNone;
	sysCalls[i363][102].res = scInt;

	//sysCalls[i363][104].name = "settimer";
	sysCalls[i363][104].types[0] = scInt;
	sysCalls[i363][104].types[1] = scNone;
	sysCalls[i363][104].types[2] = scNone;
	sysCalls[i363][104].types[3] = scNone;
	sysCalls[i363][104].types[4] = scNone;
	sysCalls[i363][104].res = scInt;

	//sysCalls[i363][117].name = "ipc";
	sysCalls[i363][117].types[0] = scNone;
	sysCalls[i363][117].types[1] = scNone;
	sysCalls[i363][117].types[2] = scNone;
	sysCalls[i363][117].types[3] = scNone;
	sysCalls[i363][117].types[4] = scNone;
	sysCalls[i363][117].res = scInt;

	//sysCalls[i363][133].name = "fchdir";
	sysCalls[i363][133].types[0] = scUIntFD;
	sysCalls[i363][133].types[1] = scNone;
	sysCalls[i363][133].types[2] = scNone;
	sysCalls[i363][133].types[3] = scNone;
	sysCalls[i363][133].types[4] = scNone;
	sysCalls[i363][133].res = scInt;

	/*//sysCalls[i363][142].name = "select";
	sysCalls[i363][142].types[0] = scUIntFD;
	sysCalls[i363][142].types[1] = scNone;
	sysCalls[i363][142].types[2] = scNone;
	sysCalls[i363][142].types[3] = scNone;
	sysCalls[i363][142].types[4] = scNone;
	sysCalls[i363][142].res = scInt;*/

	//sysCalls[i363][145].name = "readv";
	sysCalls[i363][145].types[0] = scUIntFD;
	sysCalls[i363][145].types[1] = scNone;
	sysCalls[i363][145].types[2] = scNone;
	sysCalls[i363][145].types[3] = scNone;
	sysCalls[i363][145].types[4] = scNone;
	sysCalls[i363][145].res = scInt;

	//sysCalls[i363][146].name = "writev";
	sysCalls[i363][146].types[0] = scUIntFD;
	sysCalls[i363][146].types[1] = scNone;
	sysCalls[i363][146].types[2] = scNone;
	sysCalls[i363][146].types[3] = scNone;
	sysCalls[i363][146].types[4] = scNone;
	sysCalls[i363][146].res = scInt;

	//sysCalls[i363][168].name = "poll";
	sysCalls[i363][168].types[0] = scStructPollFD;
	sysCalls[i363][168].types[1] = scULong;
	sysCalls[i363][168].types[2] = scLong;
	sysCalls[i363][168].types[3] = scNone;
	sysCalls[i363][168].types[4] = scNone;
	sysCalls[i363][168].res = scInt;

	//sysCalls[i363][183].name = "getcwd";
	sysCalls[i363][183].types[0] = scConstCharRef;
	sysCalls[i363][183].types[1] = scInt;
	sysCalls[i363][183].types[2] = scNone;
	sysCalls[i363][183].types[3] = scNone;
	sysCalls[i363][183].types[4] = scNone;
	sysCalls[i363][183].res = scRefOrNull;

	//sysCalls[i363][187].name = "sendfile";
	sysCalls[i363][187].types[0] = scUIntFD;
	sysCalls[i363][187].types[1] = scUIntFD;
	sysCalls[i363][187].types[2] = scNone;
	sysCalls[i363][187].types[3] = scSizeT;
	sysCalls[i363][187].types[4] = scNone;
	sysCalls[i363][187].res = scInt;

	//sysCalls[i363][193].name = "truncate64";
	sysCalls[i363][193].types[0] = scConstCharRef;
	sysCalls[i363][193].types[1] = scULong;
	sysCalls[i363][193].types[2] = scNone;
	sysCalls[i363][193].types[3] = scNone;
	sysCalls[i363][193].types[4] = scNone;
	sysCalls[i363][193].res = scInt;

	//sysCalls[i363][194].name = "ftruncate64";
	sysCalls[i363][194].types[0] = scUIntFD;
	sysCalls[i363][194].types[1] = scULong;
	sysCalls[i363][194].types[2] = scNone;
	sysCalls[i363][194].types[3] = scNone;
	sysCalls[i363][194].types[4] = scNone;
	sysCalls[i363][194].res = scInt;

	//sysCalls[i363][195].name = "stat64";
	sysCalls[i363][195].types[0] = scConstCharRef;
	sysCalls[i363][195].types[1] = scNone;
	sysCalls[i363][195].types[2] = scNone;
	sysCalls[i363][195].types[3] = scNone;
	sysCalls[i363][195].types[4] = scNone;
	sysCalls[i363][195].res = scInt;

	//sysCalls[i363][196].name = "lstat64";
	sysCalls[i363][196].types[0] = scConstCharRef;
	sysCalls[i363][196].types[1] = scNone;
	sysCalls[i363][196].types[2] = scNone;
	sysCalls[i363][196].types[3] = scNone;
	sysCalls[i363][196].types[4] = scNone;
	sysCalls[i363][196].res = scInt;

	//sysCalls[i363][197].name = "fstat64";
	sysCalls[i363][197].types[0] = scUIntFD;
	sysCalls[i363][197].types[1] = scNone;
	sysCalls[i363][197].types[2] = scNone;
	sysCalls[i363][197].types[3] = scNone;
	sysCalls[i363][197].types[4] = scNone;
	sysCalls[i363][197].res = scInt;

	//sysCalls[i363][221].name = "fcntl64";
	sysCalls[i363][221].types[0] = scUIntFD;
	sysCalls[i363][221].types[1] = scUInt;
	sysCalls[i363][221].types[2] = scULong;
	sysCalls[i363][221].types[3] = scNone;
	sysCalls[i363][221].types[4] = scNone;
	sysCalls[i363][221].res = scInt;

	//sysCalls[i363][239].name = "sendfile64";
	sysCalls[i363][239].types[0] = scUIntFD;
	sysCalls[i363][239].types[1] = scUIntFD;
	sysCalls[i363][239].types[2] = scNone;
	sysCalls[i363][239].types[3] = scSizeT;
	sysCalls[i363][239].types[4] = scNone;
	sysCalls[i363][239].res = scInt;

	//sysCalls[i363][256].name = "epoll_wait";
	sysCalls[i363][256].types[0] = scUIntFD;
	sysCalls[i363][256].types[1] = scNone;
	sysCalls[i363][256].types[2] = scNone;
	sysCalls[i363][256].types[3] = scNone;
	sysCalls[i363][256].types[4] = scNone;
	sysCalls[i363][256].res = scInt;


	//sysCalls[x64][0].name = "read";
	sysCalls[x64][0].types[0] = scUIntFD;
	sysCalls[x64][0].types[1] = scCharRefWithSize;
	sysCalls[x64][0].types[2] = scSizeT;
	sysCalls[x64][0].types[3] = scNone;
	sysCalls[x64][0].types[4] = scNone;
	sysCalls[x64][0].res = scInt;

	//sysCalls[x64][1].name = "write";
	sysCalls[x64][1].types[0] = scUIntFD;
	sysCalls[x64][1].types[1] = scCharRefWithSize;
	sysCalls[x64][1].types[2] = scSizeT;
	sysCalls[x64][1].types[3] = scNone;
	sysCalls[x64][1].types[4] = scNone;
	sysCalls[x64][1].res = scInt;

	//sysCalls[x64][2].name = "open";
	sysCalls[x64][2].types[0] = scConstCharRef;
	sysCalls[x64][2].types[1] = scInt;
	sysCalls[x64][2].types[2] = scInt;
	sysCalls[x64][2].types[3] = scNone;
	sysCalls[x64][2].types[4] = scNone;
	sysCalls[x64][2].res = scInt;

	//sysCalls[x64][3].name = "close";
	sysCalls[x64][3].types[0] = scUIntFD;
	sysCalls[x64][3].types[1] = scNone;
	sysCalls[x64][3].types[2] = scNone;
	sysCalls[x64][3].types[3] = scNone;
	sysCalls[x64][3].types[4] = scNone;
	sysCalls[x64][3].res = scInt;

	//sysCalls[x64][6].name = "close";
	sysCalls[x64][3].types[0] = scUIntFD;
	sysCalls[x64][3].types[1] = scNone;
	sysCalls[x64][3].types[2] = scNone;
	sysCalls[x64][3].types[3] = scNone;
	sysCalls[x64][3].types[4] = scNone;
	sysCalls[x64][3].res = scInt;

	//sysCalls[x64][4].name = "stat";
	sysCalls[x64][4].types[0] = scConstCharRef;
	sysCalls[x64][4].types[1] = scNone;
	sysCalls[x64][4].types[2] = scNone;
	sysCalls[x64][4].types[3] = scNone;
	sysCalls[x64][4].types[4] = scNone;
	sysCalls[x64][4].res = scInt;

	//sysCalls[x64][5].name = "fstat";
	sysCalls[x64][5].types[0] = scUIntFD;
	sysCalls[x64][5].types[1] = scNone;
	sysCalls[x64][5].types[2] = scNone;
	sysCalls[x64][5].types[3] = scNone;
	sysCalls[x64][5].types[4] = scNone;
	sysCalls[x64][5].res = scInt;

	//sysCalls[x64][6].name = "lstat";
	sysCalls[x64][6].types[0] = scConstCharRef;
	sysCalls[x64][6].types[1] = scNone;
	sysCalls[x64][6].types[2] = scNone;
	sysCalls[x64][6].types[3] = scNone;
	sysCalls[x64][6].types[4] = scNone;
	sysCalls[x64][6].res = scInt;

	//sysCalls[x64][7].name = "poll";
	sysCalls[x64][7].types[0] = scStructPollFD;
	sysCalls[x64][7].types[1] = scULong;
	sysCalls[x64][7].types[2] = scLong;
	sysCalls[x64][7].types[3] = scNone;
	sysCalls[x64][7].types[4] = scNone;
	sysCalls[x64][7].res = scInt;

	//sysCalls[x64][8].name = "lseek";
	sysCalls[x64][8].types[0] = scUIntFD;
	sysCalls[x64][8].types[1] = scOFFT;
	sysCalls[x64][8].types[2] = scInt;
	sysCalls[x64][8].types[3] = scNone;
	sysCalls[x64][8].types[4] = scNone;
	sysCalls[x64][8].res = scOFFT;

	//sysCalls[x64][11].name = "munmap";
	sysCalls[x64][11].types[0] = scNone;
	sysCalls[x64][11].types[1] = scNone;
	sysCalls[x64][11].types[2] = scNone;
	sysCalls[x64][11].types[3] = scNone;
	sysCalls[x64][11].types[4] = scNone;
	sysCalls[x64][11].res = scInt;

	//sysCalls[x64][12].name = "brk";
	sysCalls[x64][12].types[0] = scNone;
	sysCalls[x64][12].types[1] = scNone;
	sysCalls[x64][12].types[2] = scNone;
	sysCalls[x64][12].types[3] = scNone;
	sysCalls[x64][12].types[4] = scNone;
	sysCalls[x64][12].res = scInt;

	//sysCalls[x64][17].name = "pread64";
	sysCalls[x64][17].types[0] = scUIntFD;
	sysCalls[x64][17].types[1] = scCharRefWithSize;
	sysCalls[x64][17].types[2] = scSizeT;
	sysCalls[x64][17].types[3] = scOFFT;
	sysCalls[x64][17].types[4] = scNone;
	sysCalls[x64][17].res = scInt;

	//sysCalls[x64][18].name = "pwrite64";
	sysCalls[x64][18].types[0] = scUIntFD;
	sysCalls[x64][18].types[1] = scCharRefWithSize;
	sysCalls[x64][18].types[2] = scSizeT;
	sysCalls[x64][18].types[3] = scOFFT;
	sysCalls[x64][18].types[4] = scNone;
	sysCalls[x64][18].res = scInt;

	//sysCalls[x64][21].name = "access";
	sysCalls[x64][21].types[0] = scConstCharRef;
	sysCalls[x64][21].types[1] = scInt;
	sysCalls[x64][21].types[2] = scNone;
	sysCalls[x64][21].types[3] = scNone;
	sysCalls[x64][21].types[4] = scNone;
	sysCalls[x64][21].res = scInt;

	//sysCalls[x64][43].name = "accept";
	sysCalls[x64][43].types[0] = scUIntFD;
	sysCalls[x64][43].types[1] = scNone;
	sysCalls[x64][43].types[2] = scNone;
	sysCalls[x64][43].types[3] = scNone;
	sysCalls[x64][43].types[4] = scNone;
	sysCalls[x64][43].res = scInt;

	//sysCalls[x64][44].name = "sendto";
	sysCalls[x64][44].types[0] = scUIntFD;
	sysCalls[x64][44].types[1] = scNone;
	sysCalls[x64][44].types[2] = scNone;
	sysCalls[x64][44].types[3] = scNone;
	sysCalls[x64][44].types[4] = scNone;
	sysCalls[x64][44].res = scInt;

	//sysCalls[x64][45].name = "recvfrom";
	sysCalls[x64][45].types[0] = scUIntFD;
	sysCalls[x64][45].types[1] = scNone;
	sysCalls[x64][45].types[2] = scNone;
	sysCalls[x64][45].types[3] = scNone;
	sysCalls[x64][45].types[4] = scNone;
	sysCalls[x64][45].res = scInt;

	//sysCalls[x64][46].name = "sendmsg";
	sysCalls[x64][46].types[0] = scUIntFD;
	sysCalls[x64][46].types[1] = scNone;
	sysCalls[x64][46].types[2] = scNone;
	sysCalls[x64][46].types[3] = scNone;
	sysCalls[x64][46].types[4] = scNone;
	sysCalls[x64][46].res = scInt;

	//sysCalls[x64][47].name = "recvmsg";
	sysCalls[x64][47].types[0] = scUIntFD;
	sysCalls[x64][47].types[1] = scNone;
	sysCalls[x64][47].types[2] = scNone;
	sysCalls[x64][47].types[3] = scNone;
	sysCalls[x64][47].types[4] = scNone;
	sysCalls[x64][47].res = scInt;

	//sysCalls[x64][79].name = "getcwd";
	sysCalls[x64][79].types[0] = scNone;
	sysCalls[x64][79].types[1] = scNone;
	sysCalls[x64][79].types[2] = scNone;
	sysCalls[x64][79].types[3] = scNone;
	sysCalls[x64][79].types[4] = scNone;
	sysCalls[x64][79].res = scRefOrNull;

	//sysCalls[x64][80].name = "chdir";
	sysCalls[x64][80].types[0] = scConstCharRef;
	sysCalls[x64][80].types[1] = scNone;
	sysCalls[x64][80].types[2] = scNone;
	sysCalls[x64][80].types[3] = scNone;
	sysCalls[x64][80].types[4] = scNone;
	sysCalls[x64][80].res = scInt;

	//sysCalls[x64][81].name = "chdir";
	sysCalls[x64][81].types[0] = scUIntFD;
	sysCalls[x64][81].types[1] = scNone;
	sysCalls[x64][81].types[2] = scNone;
	sysCalls[x64][81].types[3] = scNone;
	sysCalls[x64][81].types[4] = scNone;
	sysCalls[x64][81].res = scInt;

	//sysCalls[x64][96].name = "gettimeofday";
	sysCalls[x64][96].types[0] = scNone;
	sysCalls[x64][96].types[1] = scNone;
	sysCalls[x64][96].types[2] = scNone;
	sysCalls[x64][96].types[3] = scNone;
	sysCalls[x64][96].types[4] = scNone;
	sysCalls[x64][96].res = scInt;

	//sysCalls[x64][219].name = "gettimeofday";
	sysCalls[x64][219].types[0] = scNone;
	sysCalls[x64][219].types[1] = scNone;
	sysCalls[x64][219].types[2] = scNone;
	sysCalls[x64][219].types[3] = scNone;
	sysCalls[x64][219].types[4] = scNone;
	sysCalls[x64][219].res = scInt;

	//sysCalls[x64][288].name = "accept4";
	sysCalls[x64][288].types[0] = scUIntFD;
	sysCalls[x64][288].types[1] = scNone;
	sysCalls[x64][288].types[2] = scNone;
	sysCalls[x64][288].types[3] = scNone;
	sysCalls[x64][288].types[4] = scNone;
	sysCalls[x64][288].res = scInt;

}
