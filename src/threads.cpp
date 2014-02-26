/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "threads.h"
#include "trace.h"
#include "main.h"

pthread_t workers[THREADS];
pthread_mutex_t lock;


void sigint_handler(int s){
	   if (s == SIGINT) {
		   printf("Waiting threads to finish...\n",s);
		   exitonnextloop = true;
	   }
}

void getOpenFiles(pid_t pid) {
   DIR* dir;
    struct dirent* ent;
    char* endptr;
    char buf[512], out[512];
	int pos;
	char path[100];
	string str;
	sprintf(path, "/proc/%d/fd", pid);
    if (!(dir = opendir(path))) {
        return;
    }
	std::map <unsigned int, myFileInfo> curfiles;
	curfiles.clear();
    while((ent = readdir(dir)) != NULL) {
        long fd = strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0') {
            continue;
        }

        memset(out, 0, 512);
		int type = getFileNameByDesc(out, pid, fd, NULL);

		str = out;
		if (str.substr(0, skipLogFiles.size()) != skipLogFiles && str.substr(0, skipDevNull.size()) != skipDevNull && str.substr(0, skipDevRandom.size()) != skipDevRandom &&
			str.substr(0, skipSockets.size()) != skipSockets && str.substr(0, skipEventPolls.size()) != skipEventPolls && str.substr(0, skipPipes.size()) != skipPipes && 
			str.substr(0, skipAnonInode.size()) != skipAnonInode) {
			myFileInfo of;
			sprintf(of.name, "%s", out);
			of.type = type;
			curfiles[fd] = of;
		}
    }
    if (curfiles.size() > 0) {
    	if (openFiles.find(pid) == openFiles.end()) {
    		openFile of;
    		of.files = curfiles;
			openFiles[pid] = of;
		}
	}
    closedir(dir);
}

void getProcLoad(pid_t pid, procLoad * pl) {
	ifstream fin;
	char ss[50];
	sprintf(ss, "/proc/%d/stat", pid);
	fin.open(ss);
	(*pl).uTime = 0;
	(*pl).sTime = 0;
	while (true) {
		if (!fin.good()) break;

		string lineFromFile;
		getline(fin, lineFromFile);
		sscanf(lineFromFile.c_str(), "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %llu %llu %*s", &((*pl).uTime), &((*pl).sTime));		
		break;
	}
	getCPUState(&((*pl).ci));
	fin.close();
}

void *ApacheHandler(void *threadid)
{
	int tid;
	intptr_t iptr = (intptr_t)threadid;
	tid = (int)iptr;
	pid_t child;
	int child_status, status;
	traceData traceRes[5];
	procLoad pl[2];
	getProcLoad(details[tid].pid, &pl[0]);
	int fd[2];
	int n = 0;
	if (details[tid].pid < 1) {
		cerr << "Error! PID is worng " << details[tid].pid << endl;
 	} else {
 		if (pipe(fd) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
        }
		switch(child = fork()) {
				case 0:
						close(fd[0]);
						traceProcess(details[tid].pid, &traceRes);
						write(fd[1], traceRes, sizeof(traceRes));
						close(fd[1]);
						exit(42);
				case -1:
						cerr << "Can't FORK" << endl;
						break;
				default:
						close(fd[1]);
						details[tid].workerpid = child;
						break;
		}
		
		while (1) {
				usleep(1);
				status = waitpid(child, &child_status, WNOHANG);
				switch (status) {
						case -1:
								/* waitpid() error */
								cerr << "waitpid error, pid: "<< details[tid].pid << endl;
								break;
						case 0:
								/* child hasn't exited yet */
								break;
						default:
								pthread_mutex_lock(&lock);
								n = read(fd[0], &(details[tid].traces), sizeof (details[tid].traces));
								activeThreads--;
								getOpenFiles(details[tid].pid);
								details[tid].cpu = 0;
								
								pthread_mutex_unlock(&lock);
								close(fd[0]);
								details[tid].done = true;
								details[tid].workerpid = 0;
								return NULL;
								break;
				}
				usleep(timeout / 10);
		}
	}
	activeThreads--;
	workers[tid] = 0;
	details[tid].done = true;
	details[tid].workerpid = 0;
	close(fd[1]);
	close(fd[0]);
	return NULL;
}

