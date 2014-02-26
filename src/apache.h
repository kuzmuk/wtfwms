/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __APACHE_H
#define __APACHE_H


#include "threads.h"
#include "trace.h"
#include <curl/curl.h>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include "cpuload.h"

struct apacheDetails {
	unsigned int pid;
	char status[2];
	char ip[32];
	char url[64];
	float cpu;
	int duration;
	char querytype[10];
	char vhost[32];
	bool done;
	pid_t workerpid;
	traceData traces[5];
};

struct myFileInfo {
	char	name[250];
	unsigned short type;

};


struct openFile {
	std::map <unsigned int, myFileInfo> files;
};


extern std::map <unsigned int, openFile> openFiles;

extern apacheDetails details[THREADS];
extern unsigned int activeApacheProcesses;

void getApacheStatus();
int findActiveProcesses();
void reset();

#endif
