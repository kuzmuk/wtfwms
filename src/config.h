/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __CONFIG_H

#define __CONFIG_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib>
#include <list>

using namespace std;

extern string statusURL, skipmeURL, serverPostDataURL, hostname, APIKey, db_user, db_password, skipLogFiles, skipDevNull, skipDevRandom, workersName, skipSockets, skipEventPolls, skipPipes, skipAnonInode;
extern unsigned int timeout, listenforms, read_limit, reportDelay, threshold;
extern bool obfuscatesql, fetchdataonread, fetchdataonwrite, useModStatus, resolvenetsockets,resolveunixsockets, hideoutput;
extern list<string> mysqlShards;

void readconfig();

#endif
