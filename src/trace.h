/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __TRACE_H

#define __TRACE_H

using namespace std;

#include <sys/types.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/user.h>   /* For user_regs_struct etc. */
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <limits>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <netdb.h>
#include <sys/socket.h>
#include <linux/net.h>
#include <dirent.h>
#include <cstdlib>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __x86_64__ 
#define SC_NUMBER  (8 * ORIG_RAX)
#define SC_RETCODE (8 * RAX)
#define SC_EBX (8 * RDI)
#define SC_ECX (8 * RSI)
#define SC_EDX (8 * RDX)
#define SC_ESI (8 * R10)
#define SC_EDI (8 * R8)
#define SC_SIZE 8
#else
#define SC_NUMBER  (4 * ORIG_EAX)
#define SC_RETCODE (4 * EAX)
#define SC_EBX (4 * EBX)
#define SC_ECX (4 * ECX)
#define SC_EDX (4 * EDX)
#define SC_ESI (4 * ESI)
#define SC_EDI (4 * EDI)
#define SC_SIZE 4
#endif



struct traceData {
	char sparams[5][512];
	long lparams[5];
	long systemCallId;
	int result;
	struct timeval time;
};
enum {ftUnknown, ftFile, ftTCP, ftSocket, ftPipe};
struct socketMeta {
	string name;
	int type;
};

void traceProcess(int pid, traceData (*tdp)[5]);
void print_desc(pid_t pid, long sc_number, long sc_retcode, struct user_regs_struct regs);
void getProcessData(pid_t child, traceData* td, map <unsigned long,socketMeta> * socketCache);
void fetchData(pid_t child, traceData* td, short i, map <unsigned long,socketMeta> * socketCache);
void getContData(pid_t child, long addr, char *str);
void getdata(pid_t child, long addr, char *str, int len);
void print_fdesc(ostringstream* stringStream, pid_t pid, int fd, map <unsigned long,socketMeta> * socketCache);
void fixStoppedProc(int pid);
char getProcStatus(int pid);
void fixStoppedProcesses();
int getFileNameByDesc(char strout[512], pid_t pid, int fd, map <unsigned long,socketMeta> * socketCache);

extern std::map <unsigned long,string> networkNamesCache;

#endif

