/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __CPULOAD_H
#define __CPULOAD_H

#include <sys/time.h>
#include <stddef.h>
#include <fstream>
#include <iostream>
#include <map>
#include <dirent.h>
#include "threads.h"
#include "apache.h"
#include "config.h"

using namespace std;

struct cpuInfo {
	unsigned long user, nice, system, idle, iowait, irq, softirq, steal, guest;
	int cpucount;
	struct timeval time;
};

struct memInfo {
	unsigned long freeMem, totalMem, freeSwap, totalSwap;
};

struct procLoad {
	unsigned long uTime, sTime;
	cpuInfo ci;
};

struct processInfo {
	pid_t pid;
	char state;
	string name;
	unsigned long uTime, sTime;
	cpuInfo ci;
	double cpuLoad;
};

extern std::map <pid_t, processInfo> processes;


extern cpuInfo CPUState[2];
extern memInfo memStat;

void getCPUState(cpuInfo * c);
double ParseCPUDataAndGetTotal();
double getIOWait();
void getLA(string * str);	
void getMemState();
bool getProcInfo(pid_t pid, bool again);
int getProcessesList(bool again);

#endif

