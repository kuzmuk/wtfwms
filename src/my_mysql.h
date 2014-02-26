/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/


#ifndef __MY_MYSQL_H
#define __MY_MYSQL_H

#include <sys/time.h>
#include <mysql.h>
#include <stdio.h>
#include <string>
#include "config.h"
#include <map>
#include <vector>


struct sqlProcess {
	string id;
	string shard;
	string user;
	string host;
	string db;
	string commandtype;
	string time;
	string state;
	string info;
};


extern std::map <string, sqlProcess> queries;

void getMySQLProcessList();
void getProcessesFromShard(string shard);

#endif
