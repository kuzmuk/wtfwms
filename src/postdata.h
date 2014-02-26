/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __POSTDATA_H
#define __POSTDATA_H


#include "threads.h"
#include <curl/curl.h>
#include <sstream>
#include "my_mysql.h"
#include <map>
#include "apache.h"
#include "main.h"
#include "cpuload.h"
#include <algorithm>


void postData();
void sanitizeSQL(string * sql, char c);

#endif
