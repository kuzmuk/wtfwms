/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "trace.h"
#include "config.h"
#include "syscalls.h"
#include "netlink.h"


const int long_size = sizeof(long);
const int int_size = sizeof(int);

map <unsigned long,string> networkNamesCache;


void traceProcess(int pid, traceData (*tdp)[5]) {
	map <unsigned long,socketMeta> socketCache;
	traceData curTrace;
	traceData traces[5];
	pid_t traced_process, child_pid;
	long sc_number, sc_retcode;
	int status;
	struct timeval tv;
	long start_time, cur_time, start_secs;
	string output;
	int tracesCount = 0;
	traced_process = pid;
	int error = 0;
	bool firstStop = true, sentstopsignal = false;

	ptrace(PTRACE_ATTACH, traced_process, NULL);
	curTrace.systemCallId = -1;
	for (int i = 0; i<5; i++) {
		traces[i] = curTrace;
	}
	gettimeofday(&tv, NULL);
	start_secs = tv.tv_sec;
	start_time = (tv.tv_usec) / 1000;
	gettimeofday(&tv, NULL);
	socketCache.clear();
	cur_time = (tv.tv_sec - start_secs) * 1000 + (tv.tv_usec) / 1000 ;
	while (1) {
		/* Wait for child status to change: */
		child_pid = waitpid( traced_process /* or -1 */, &status, __WALL | WNOHANG);
		if (child_pid > 0) {
			if (WIFEXITED(status)) {
					break;
			}
			if (WIFSIGNALED(status)) {
					printf("Child exit due to signal %d\n", WTERMSIG(status));
					break;
			}
			if (!WIFSTOPPED(status)) {
					printf("wait() returned unhandled status 0x%x\n", status);
					break;
			}
			if (WSTOPSIG(status) == SIGTRAP) {
				/* Note that there are *three* reasons why the child might stop
				 * with SIGTRAP:
				 *  1) syscall entry
				 *  2) syscall exit
				 *  3) child calls exec
				 */
					sc_number = ptrace(PTRACE_PEEKUSER, traced_process, SC_NUMBER, NULL);
					sc_retcode = ptrace(PTRACE_PEEKUSER, traced_process, SC_RETCODE, NULL);

					if (sc_number >= 0 && sc_number < 312) {
						curTrace.systemCallId = sc_number;
						curTrace.result = sc_retcode;

						getProcessData(child_pid, &curTrace, &socketCache);

						if (sc_retcode != -ENOSYS && traces[0].systemCallId == sc_number) { //finished
							traces[0].result = sc_retcode;
							gettimeofday(&traces[0].time, NULL);
						} else {
							curTrace.result = sc_retcode; 
							gettimeofday(&curTrace.time, NULL);
							traces[4] = traces[3];
							traces[3] = traces[2];
							traces[2] = traces[1];
							traces[1] = traces[0];
							traces[0] = curTrace;
							tracesCount++;
						}
					}

			} else {
				if (WSTOPSIG(status) == SIGSTOP) {
					if (!firstStop) {
						error = ptrace(PTRACE_DETACH, traced_process, (char *) 1, 0);
						break;
					} else {
						firstStop = false;
					}
				}
			}
			error = ptrace(PTRACE_SYSCALL, traced_process, NULL, NULL);
		}
		if (child_pid < 0) {
			break;
		}
		gettimeofday(&tv, NULL);
		cur_time = (tv.tv_sec - start_secs) * 1000 + (tv.tv_usec) / 1000 ;
		if ((cur_time >= start_time + listenforms || tracesCount > 100) && !sentstopsignal) {

			if (kill(pid, 0) < 0) {
				if (errno != ESRCH)
					perror("detach: checking sanity");
			} else {
				if (kill(pid, SIGSTOP) < 0) {
					if (errno != ESRCH)
						perror("detach: stopping child");
				} else {
					usleep(1);
					sentstopsignal = true;
				}
			}
		}
	}
	socketCache.clear();
	for (int j=0; j < 5; j++) {
		(*tdp)[j] = traces[j];
	}
}

char getProcStatus(int pid) {
	ifstream fin;
	char ss[200];
	char param[50], value[150];
	sprintf(ss, "/proc/%d/status", pid);
	fin.open(ss);
	string str = "State:", p;
	while (true) {
		if (!fin.good()) break;

		string lineFromFile;
		getline(fin, lineFromFile);
		sscanf(lineFromFile.c_str(), "%s\t%s", param, value);
		p = param;
		if (str.compare(p) == 0) {
			fin.close();
			return value[0];
		}
	}
	fin.close();
	return ' ';
}

void fixStoppedProc(int pid) {
	cout << "kill(pid, SIGCONT); " << pid << endl;
	int error = kill(pid, 0);
	if (error != 0) {
		perror("kill 0:");
	}
	error = kill(pid, SIGCONT);
	if (error != 0) {
		if (errno != ESRCH)
			perror("fix STOP: kill(pid, SIGCONT)");
	}
}

void fixStoppedProcesses()
{	
	DIR* dir;
	struct dirent* ent;
	char* endptr;
	char buf[512];
	unsigned long pos;
	ifstream fin;
	string lineFromFile;
	string name, val;

	if (!(dir = opendir("/proc"))) {
		perror("Can't open /proc");
		return;
	}
	while((ent = readdir(dir)) != NULL) {
		long lpid = strtol(ent->d_name, &endptr, 10);
		if (*endptr != '\0') {
			continue;
		}

		snprintf(buf, sizeof(buf), "/proc/%ld/status", lpid);

		fin.open(buf);
		while (true) {
			if (!fin.good()) break;
			getline(fin, lineFromFile);
			pos = lineFromFile.find("\t");
			if (pos != std::string::npos) {
				name = lineFromFile.substr(0, pos - 1);
				val = lineFromFile.substr(pos+1);
			} else {
				break;
			}
			if (name.compare("Name") == 0) {
				if (val.compare(workersName) != 0) {
					break;
				}
			}
			if (name.compare("State") == 0) {
				if (val[0] == 'T') {
					fixStoppedProc((pid_t)lpid);
					//cout << name << ": " << val <<endl;
				}
				break;
			}
		}
		fin.close();
	}
	closedir(dir);
}


int ip_hex_to_dquad(  unsigned long a,  unsigned short p, char *output, size_t outlen) {
	char host[1024];
	char service[20];
	char ip[9];
	char *str;

	struct sockaddr_in sa;
	memset(&sa,0,sizeof(sa));
	memset(host,0,sizeof(host));
	memset(service,0,sizeof(service));
	map <unsigned long,string>::iterator it = networkNamesCache.find(p);
	if (it != networkNamesCache.end()) {		
		sprintf(service, "%s", it->second.c_str());
	} else {
		sprintf(service, "%d", p);
	}
	it = networkNamesCache.find(a);
	if (it != networkNamesCache.end()) {	
		sprintf(host, "%s", it->second.c_str());
	} else {
		sa.sin_family = AF_INET; // IP4
		sa.sin_port = htons(p);
		sa.sin_addr.s_addr = a;
		str = inet_ntoa(sa.sin_addr);
		sprintf(host, "%s", str);
		
		networkNamesCache[a] = host;
		networkNamesCache[p] = service;
	}

	snprintf(output, outlen, "%s:%s", host, service);

	return 0;
}

void print_fdesc(traceData* td, pid_t pid, int i, map <unsigned long,socketMeta> * socketCache) {
	int fd = (unsigned int)(*td).lparams[i];
	char * strparam;
	strparam = (*td).sparams[i];
	getFileNameByDesc(strparam, pid, fd, socketCache);
}


int getFileNameByDesc(char * strout, pid_t pid, int fd, map <unsigned long,socketMeta> * socketCache) {
	char buf[30];
	char out[255];
	unsigned long socketid;

	ssize_t len;
	memset(buf, 0, 30);
	sprintf(buf, "/proc/%d/fd/%d",	pid, fd);
	memset(strout, 0, 512);
	memset(out, 0, 30);
	len = readlink(buf, out, sizeof(out)-1);
    if (len > 0) {
	    out[len] = 0;
		if (out[0] == 's' && out[1] == 'o' && out[2] == 'c' && out[3] == 'k' && out[4] == 'e' && out[5] == 't'  && out[6] == ':') {
			string str = out;
			char pidIn[16], desc[255], addrlocal[250], addrremote[250], status[15];
			str = str.substr(8, str.length()-9);
			socketid = atol(str.c_str());
			if (socketid > 0) {
				if (socketCache != NULL) {
					map <unsigned long,socketMeta>::iterator it = (*socketCache).find(socketid);
					if (it != (*socketCache).end()) {
						sprintf(strout, "%s",it->second.name.c_str());
						return it->second.type;
					}
				}

				if (resolvenetsockets) {
					inet_diag_msg data;
					int res = tcp_netlink(&data, socketid);
					if (res > 0) {
						string str;
						ip_hex_to_dquad(data.id.idiag_src[0], ntohs(data.id.idiag_sport), addrlocal, 250);
						ip_hex_to_dquad(data.id.idiag_dst[0], ntohs(data.id.idiag_dport), addrremote, 250);
						sprintf(strout, "%s->%s", addrlocal, addrremote);
						if (socketCache != NULL) {
							socketMeta sm;
							sm.name = strout;
							sm.type = ftTCP;
							(*socketCache)[socketid] = sm;
						}
						return ftTCP;
					}
				}
				if (resolveunixsockets) {
					ifstream fin;
					memset(buf, 0, 30);
					sprintf(buf, "/proc/%d/net/unix", pid);
					fin.open(buf);
					string lineFromFile;
					while (true) {
						if (!fin.good()) break;

						getline(fin, lineFromFile);
						sscanf(lineFromFile.c_str(), "%*s %*s %*s %*s %*s %*s %s %s", pidIn, desc);
						if (str.compare(pidIn) == 0) {
							if (socketCache != NULL) {
								socketMeta sm;
								sm.name = desc;
								sm.type = ftSocket;
								(*socketCache)[socketid] = sm;
							}
							sprintf(strout, "%s",desc);
							fin.close();
							return ftSocket;
						}
					}
					fin.close();
				}
				if (socketCache != NULL) {
					socketMeta sm;
					sm.name = out;
					sm.type = ftUnknown;
					(*socketCache)[socketid] = sm;
				}
			}
		}
		sprintf(strout, "%s",out);
		return (out[0] == '/') ? ftFile : ftUnknown;
	}
	return ftUnknown;
}


void getProcessData(pid_t child, traceData* td, map <unsigned long,socketMeta> * socketCache) {	
	(*td).lparams[0] = ptrace(PTRACE_PEEKUSER, child, SC_EBX , NULL);
	(*td).lparams[1] = ptrace(PTRACE_PEEKUSER, child, SC_ECX , NULL);
	(*td).lparams[2] = ptrace(PTRACE_PEEKUSER, child, SC_EDX , NULL);
	(*td).lparams[3] = ptrace(PTRACE_PEEKUSER, child, SC_ESI , NULL);
	(*td).lparams[4] = ptrace(PTRACE_PEEKUSER, child, SC_EDI , NULL);
	for (int i = 0; i < 5; i++) {
		(*td).sparams[i][0] = 0;
		fetchData(child, td, i, socketCache);
	}
}

void fetchData(pid_t child, traceData* td, short i, map <unsigned long,socketMeta> * socketCache) {
	unsigned int nextdata;

	switch (sysCalls[PLATFORM][(*td).systemCallId].types[i]) {
		case scPIDt:
		case scUInt:
			sprintf((*td).sparams[i], "%u", (unsigned int)(*td).lparams[i]);
			break;
		case scIntRef:
			int * addr;
			addr = (int *)((*td).lparams[i]);
			(*td).lparams[i] = (int)(*addr);
			break;
		case scUIntFD:
			print_fdesc(td, child, i, socketCache);
			break;
		case scOFFT:
		case scULong:
			sprintf((*td).sparams[i], "%u", (unsigned long)(*td).lparams[i]);
			break;
		case scSizeT:
			sprintf((*td).sparams[i], "%d", (int)(*td).lparams[i]);
			break;
		case scInt:
			sprintf((*td).sparams[i], "%d", (int)(*td).lparams[i]);
			break;
		case scConstCharRef:
			getContData(child, (*td).lparams[i], (*td).sparams[i]);
			break;
		case scCharRefWithSize:
			nextdata = (*td).lparams[i+1];
			if (nextdata > read_limit-1) {
				nextdata = read_limit-1;
			}

			getdata(child, (*td).lparams[i], (*td).sparams[i], nextdata);
			break;
		case scNone:
		default:
			break;
	}
}


void getContData(pid_t child, long addr, char *str) {
	char *laddr;
    unsigned int i, j;
    union u {
            long val;
            char chars[long_size];
    }data;
    i = 0;
    j = read_limit / long_size;
    laddr = str;
    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, data.chars);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = read_limit % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA,
                          child, addr + i * SC_SIZE,
                          data.chars);
        memcpy(laddr, data.chars, j);
    }
    for (i=0; i<= read_limit; i++) {
    	if (str[i] < 32) {
    		if (str[i] == 0) break;
    		str[i] = '_';
			if (str[i] == '&') {
				str[i] = '_';
			}
			if (str[i] == '|') {
				str[i] = '_';
			}
    	}
    }
    str[read_limit-1] = '\0';
}



void getdata(pid_t child, long addr, char *str, int len) {
	char *laddr;
    int i, j;
    union u {
            long val;
            char chars[long_size];
    }data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, data.chars);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, data.chars);
        memcpy(laddr, data.chars, j);
    }
    for (i=0; i<= len; i++) {
    	if (str[i] < 32) {
    		str[i] = '_';
    	}
    	if (str[i] == '&') {
    		str[i] = '_';
    	}
    	if (str[i] == '|') {
    		str[i] = '_';
    	}
    }
    str[len] = '\0';
}


