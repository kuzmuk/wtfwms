/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __SYSCALLS_H

#define __SYSCALLS_H

using namespace std;

#include <sys/types.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/user.h>   /* For user_regs_struct etc. */
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string>
#include <sys/time.h>


enum { scNone, scUInt, scInt, scConstCharRef, scCharRef, scSizeT, scPidT, scUIntRef, scSctructPtRegs, scIntRef,
	   scModeT, scUIDT, scOFFT, scLong, scULong, scULongRef, scStructPollFD, scNfdsT, scCharRefWithSize, scUIntFD,
	   scULongSocketRef, scPIDt, scRefOrNull };

struct syscallsMeta {
	unsigned short types[5], res;
	char * name;
};

extern syscallsMeta sysCalls[2][312];
void initSysCalls ();


#ifdef __x86_64__
#define PLATFORM 1
#else
#define PLATFORM 0
#endif

#define x64  1
#define i363 0

#endif

