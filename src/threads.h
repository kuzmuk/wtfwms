/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#ifndef __THREADS_H
#define __THREADS_H


#define THREADS 32
using namespace std;
#include <pthread.h>
#include <iostream>
#include "apache.h"
#include <csignal>
#include <cstdlib>
#include <dirent.h>
#include <inttypes.h>

extern pthread_t workers[THREADS];
extern pthread_mutex_t lock;

void *ApacheHandler(void *threadid);
void sigint_handler(int s);





#endif
