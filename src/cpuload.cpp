/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "cpuload.h"

cpuInfo CPUState[2];
memInfo memStat;
std::map <pid_t, processInfo> processes;


bool getProcInfo(pid_t pid, bool again) {
	ifstream fin;
	processInfo p;
	char ss[50];
	char cmdline[50];
	char state;
	sprintf(ss, "/proc/%d/stat", pid);
	fin.open(ss);
	p.uTime = 0;
	p.sTime = 0;
	p.cpuLoad = 0;
	bool found = false;
	while (true) {
		if (!fin.good()) break;

		string lineFromFile;
		getline(fin, lineFromFile);
		sscanf(lineFromFile.c_str(), "%*s %s %c %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %llu %llu %*s", cmdline, &p.state, &(p.uTime), &(p.sTime));		
		p.name = cmdline;
		p.name = p.name.substr(1, p.name.size()-2);
		if (p.name.compare(workersName) == 0 && (p.state == 'R' || p.state == 'D')) {
			found = true;
		}
		break;
	}
	getCPUState(&(p.ci));
	fin.close();
	if (again) {
		map <pid_t, processInfo>::iterator it = processes.find(pid);
		if (it != processes.end()) {		
			if (p.name.compare(it->second.name) == 0) {
				unsigned long dtU, dtS, t1, t2;
				dtU = p.uTime - it->second.uTime;
				dtS = p.sTime - it->second.sTime;
				t1 = it->second.ci.user + it->second.ci.nice + it->second.ci.system + it->second.ci.idle + it->second.ci.iowait + it->second.ci.irq+ it->second.ci.softirq + it->second.ci.steal + it->second.ci.guest;
				t2 = p.ci.user + p.ci.nice + p.ci.system + p.ci.idle + p.ci.iowait + p.ci.irq+ p.ci.softirq + p.ci.steal + p.ci.guest;
				double load = 100.0 * (dtU+dtS) / (t2-t1);
				it->second.cpuLoad = load;
			}
		}
	} else {
		processes[pid] = p;
	}
	return found;
}

int getProcessesList(bool again) {
	int foundRows = 0;
    DIR* dir;
    struct dirent* ent;
    char* endptr;
    char buf[512];
    char ss[100];
	int pos;
	ifstream fin;
    string lineFromFile;
    string name, val;
    if (!again) {
    	processes.clear();
	}
    if (!(dir = opendir("/proc"))) {
        perror("Can't open /proc");
        return 0;
    }
    while((ent = readdir(dir)) != NULL && foundRows < THREADS) {
        long lpid = strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0') {
            continue;
        }
        if (getProcInfo(lpid, again)) {
        	if (foundRows < THREADS && !again) {
	    		details[foundRows].pid = lpid;
	        	foundRows++;
        	}
        }
    }
    closedir(dir);
    return foundRows;
}

double ParseCPUDataAndGetTotal() {
	unsigned long sum;
	CPUState[1].user -= CPUState[0].user;
	CPUState[1].nice -= CPUState[0].nice;
	CPUState[1].system -= CPUState[0].system;
	CPUState[1].idle -= CPUState[0].idle;
	CPUState[1].iowait -= CPUState[0].iowait;
	CPUState[1].irq -= CPUState[0].irq;
	CPUState[1].softirq -= CPUState[0].softirq;
	CPUState[1].steal -= CPUState[0].steal;
	CPUState[1].guest -= CPUState[0].guest;

	sum = CPUState[1].user + CPUState[1].nice + CPUState[1].system + CPUState[1].idle + CPUState[1].iowait + CPUState[1].irq+ CPUState[1].softirq + CPUState[1].steal+CPUState[1].guest;
	double x = 100.0 * (sum - CPUState[1].idle);
	x = x / sum;
	return x;
}

double getIOWait() {
	unsigned long sum = CPUState[1].user + CPUState[1].nice + CPUState[1].system + CPUState[1].idle + CPUState[1].iowait + CPUState[1].irq+ CPUState[1].softirq + CPUState[1].steal+CPUState[1].guest;
	double x = 100.0 * (CPUState[1].iowait);
	x = x / sum;
	return x;
}

void getCPUState(cpuInfo * c) {
	string line;
	cpuInfo cp;
	string cpuN;
	char buf[100];
    std::ifstream filein("/proc/stat");

	cp.user = 0;
	cp.nice = 0;
	cp.system = 0;
	cp.idle = 0;
	cp.iowait = 0;
	cp.irq = 0;
	cp.softirq = 0;
	cp.steal = 0;
	cp.guest = 0;
	cp.cpucount = 0;
	(*c).user = 0;
	(*c).nice = 0;
	(*c).system = 0;
	(*c).idle = 0;
	(*c).iowait = 0;
	(*c).irq = 0;
	(*c).softirq = 0;
	(*c).steal = 0;
	(*c).guest = 0;
	(*c).cpucount = 0;

    for (std::string line; std::getline(filein, line); )
    {
    	buf[0] = 0;
        sscanf(line.c_str(), "%s %d %d %d %d %d %d %d %d %d", buf, &cp.user, &cp.nice, &cp.system, &cp.idle, &cp.iowait, &cp.irq, &cp.softirq, &cp.steal, &cp.guest);
        cpuN = buf;
        if (cpuN.substr(0, 3).compare("cpu") == 0 && cpuN.size() == 3) {
			(*c).user += cp.user;
			(*c).nice += cp.nice;
			(*c).system += cp.system;
			(*c).idle += cp.idle;
			(*c).iowait += cp.iowait;
			(*c).irq += cp.irq;
			(*c).softirq += cp.softirq;
			(*c).steal += cp.steal;
			(*c).guest += cp.guest;
			(*c).cpucount = 0;
        } else if (cpuN.substr(0, 3).compare("cpu") == 0 && cpuN.size() > 3) { 
        	(*c).cpucount++;
    	} else {
        		break;
    	}
    }
    filein.close();
	gettimeofday(&((*c).time), NULL);
}

void getMemState() {
	string line;
    std::ifstream filein("/proc/meminfo");
    char buf[100];
    char buf2[10];
    string s;
    unsigned long v;
	memStat.freeMem = 0;
	memStat.totalMem = 0;
	memStat.freeSwap = 0;
	memStat.totalSwap = 0;

    for (std::string line; std::getline(filein, line); ) {
    	buf[0] = 0;
    	buf2[0] = 0;
    	v = 0;
        sscanf(line.c_str(), "%s %d %s", buf, &v, buf2);
        s = buf;
        if (s.compare("MemTotal:") == 0) {
        	memStat.totalMem = v;
        } else if (s.compare("MemFree:") == 0) {
        	memStat.freeMem = v;
        } else if (s.compare("SwapTotal:") == 0) {
        	memStat.totalSwap = v;
        } else if (s.compare("SwapFree:") == 0) {
        	memStat.freeSwap = v;
        	break;
        }
    }
    filein.close();
}


void getLA(string * str) {
	string line;
    std::ifstream filein("/proc/loadavg");
    for (std::string line; std::getline(filein, line); ) {
    	(*str) = line;
    	break;
    }
    filein.close();
}
