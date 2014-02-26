/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/


#ifndef __MAIN_H

#define __MAIN_H

using namespace std;

#include "apache.h"
#include "threads.h"
#include "config.h"
#include "trace.h"
#include "syscalls.h"
#include "postdata.h"
#include "my_mysql.h"
#include "cpuload.h"
#include <sys/prctl.h>
#include <net/if.h>
#include <sys/ioctl.h>

extern int activeThreads;
extern unsigned char cMacAddr[8];
extern char macAddress[17];
int main(int argc, char *argv[]);
extern bool exitonnextloop;
static int GetSvrMacAddress( char *pIface );
void purgeNetworkNamesCache();
#endif
