/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "postdata.h"

template<class T>
	struct SortByTime: std::binary_function<T,T,bool>
	{
		inline bool operator ()(const T& _left, const T& _right)
		{
			return _left.second.time > _right.second.time;
		}
	};

template<class T>
	struct SortByLoad: std::binary_function<T,T,bool>
	{
		inline bool operator ()(const T& _left, const T& _right)
		{
			return _left.second.cpuLoad > _right.second.cpuLoad;
		}
	};



typedef std::pair<string, sqlProcess> SQL_pair;
typedef std::pair <pid_t, processInfo> ProcPair;
void sanitizeSQL(string * sql, char c) {
	string key, mask;
	mask = "%";
	bool found;
	string cmp, cmp2;
	char cmpc[3], cmpc2[4];
	cmpc[0] = '\\';
	cmpc[1] = c;
	cmpc[2] = 0;
	cmpc2[0] = c;
	cmpc2[1] = '%';
	cmpc2[2] = c;
	cmpc2[3] = 0;
	cmp = cmpc;
	cmp2 = cmpc2;
	int pos, last = 0, pos_prev = -1;
	found = true;
	while (found) {
		found = false;
		pos = (*sql).find("\\\'");
		if (pos!=std::string::npos) {
			(*sql).replace(pos, 2, "  ");
			found = true;
		}
		pos = (*sql).find("\\\"");
		if (pos!=std::string::npos) {
			(*sql).replace(pos, 2, "  ");
			found = true;
		}
	}
	found = true;
	while (found) {
		found = false;
		pos = (*sql).find(c, last);
		if (pos!=std::string::npos) {
			if ((*sql).substr(pos-1, 2).compare(cmp) != 0) {
				if (pos_prev > -1) {
					if ((*sql).substr(pos_prev, 3).compare(cmp2) != 0) {
						(*sql) = (*sql).substr(0, pos_prev+1).append(mask.append((*sql).substr(pos)));
						mask = "%";
						pos = -1;
						pos_prev = -1;
					} else {
						pos_prev = -1;
					}
				} else {
					pos_prev = pos;
				}
			}
			last = pos+1;
			found = true;
		}
	}
	if (c == '\"') return;
	char curchar, c1,c2;
	for (int i =0; i<(*sql).size(); i++) {
		curchar = (*sql)[i];
		if (i > 0 && curchar == ' ') {
			if ((*sql)[i-1] == ' ') {
				(*sql) = (*sql).substr(0, i-1).append((*sql).substr(i));
				i--;
			}
		}
		if (i > 2 && (curchar >= '0' && curchar <= '9')) {
				c1 = (*sql)[i-1];
				 c2 = (*sql)[i-2];
			if (c1 == ' ' && (c2 == '=' || c2 == '<' || c2 == '>' || c2 == '(' || c2 == ',' || c2 == '(' || c2 == 'T' || c2 == 't') ||
							 (c1 == '=' || c1 == '<' || c1 == '>' || c1 == '(' || c1 == ',' || c1 == '(')) {
				(*sql)[i] = '%';
			} else {
				if (c1 == '%') {
					(*sql) = (*sql).substr(0, i).append((*sql).substr(i+1));
					i--;
				}
			}
		}
	}
}

void encode(string * str) {
	long found = 1;
	string p1, p2;
	found = (*str).find("+");
	while (found != std::string::npos) {
		p1 = (*str).substr(0, found);
		p2 = (*str).substr(found+1);
		*str = p1.append("%2B").append(p2);
		found = (*str).find("+");
	}	
}
int writerc(char *data, size_t size, size_t nmemb, string *buffer){
	return size * nmemb;
}
void postData() {
	 struct tm* ptm;
 	char time_string[40], timestr[60];
	CURL *curl;
	CURLcode res;
	std::ostringstream stringStream;
	string post;
 	long milliseconds;
 	
 	stringStream << "HOSTNAME=" << hostname;
	stringStream << "&APIKEY=" << APIKey;
	stringStream.precision(2);
	stringStream << "&CPULOAD=" << fixed << ParseCPUDataAndGetTotal();
	stringStream << "&IOWAIT=" << fixed << getIOWait();
	stringStream << "&MACADDR=" << macAddress;
	stringStream << "&ARCH=" << (PLATFORM ? "x86_64" : "i386");
	stringStream << "&CPU=" << CPUState[0].cpucount;
	string la, sql;
	getLA(&la);
	stringStream << "&LA=" << la;
	getMemState();
	double memUsage = ((memStat.totalMem - memStat.freeMem) * 100 / memStat.totalMem);
	double swapUsage = 0;
	if (memStat.totalSwap > 0) {
		swapUsage = ((memStat.totalSwap - memStat.freeSwap) * 100 / memStat.totalSwap);
	}
	stringStream << "&MEM=" << memUsage << "|" << swapUsage;

	map <pid_t, processInfo>::iterator it;
	for (unsigned j = 0; j< activeApacheProcesses && j < THREADS; j++) {
		double load = 0;
		it = processes.find(details[j].pid);
		if (it != processes.end()) {		
			load = it->second.cpuLoad;
		}
		stringStream << "&PIDS[" << details[j].pid << "]=" << details[j].ip << "|" << details[j].vhost << "|"  << details[j].querytype << "|" << details[j].url << "|" <<  fixed << load << "|" <<  details[j].duration;
	}

	for (unsigned j = 0; j< activeApacheProcesses && j < THREADS; j++) {
		stringStream << "&TRACES[" << details[j].pid << "]=";
		for (int jj=0; jj <5; jj++) {
			if (details[j].traces[jj].systemCallId > -1 && details[j].traces[jj].systemCallId  < 312) {
				stringStream << details[j].traces[jj].systemCallId << "|";
				for (int jjj = 0; jjj <5; jjj++) {
					if (sysCalls[PLATFORM][details[j].traces[jj].systemCallId].types[jjj] != scNone) {
						if (strlen(details[j].traces[jj].sparams[jjj]) > 0) {
							sql = details[j].traces[jj].sparams[jjj];
							if (sql.size() > 5 && sql.substr(1, 4).compare("____") == 0 && sql.substr(5, 1).compare("_") != 0) {
								sql.replace(0, 1, "_");
								if (obfuscatesql) {
									sanitizeSQL(&sql, '\'');
									sanitizeSQL(&sql, '\"');
								}
								encode(&sql);
							}
							stringStream << sql << "|";
						} else {
							stringStream << details[j].traces[jj].lparams[jjj] << "|";
						}
					} else {
						stringStream << "|";
					}
				}

				stringStream << details[j].traces[jj].result << "|";
				ptm = localtime (&details[j].traces[jj].time.tv_sec);
				strftime (time_string, sizeof (time_string), "%H:%M:%S", ptm);
 				milliseconds = details[j].traces[jj].time.tv_usec;
 				sprintf (timestr, "%s.%06ld", time_string, milliseconds);
				stringStream << timestr << endl;
			}
		}
	}

    std::vector<SQL_pair> myvec(queries.begin(), queries.end());
    std::sort(myvec.begin(), myvec.end(), SortByTime<SQL_pair>());

	map <string, int> files;
	files.clear();
	
	for (std::map <unsigned int, openFile>::iterator it = openFiles.begin(); it != openFiles.end(); ++it)
	{
		for (std::map <unsigned int, myFileInfo>::iterator it2 = it->second.files.begin(); it2 != it->second.files.end(); ++it2) {
			if (strlen(it2->second.name) > 0) {
				stringStream << "&FD[" << it->first << "][" << it2->first <<"]=";
				stringStream << it2->second.name << "|";
				stringStream << it2->second.type;
				if (it2->second.type == ftTCP) {
					string filename = it2->second.name;
					size_t found =  filename.find("->");
					if (found!=std::string::npos) {
						filename = filename.substr(0, found);
					}
					files[filename] = 1;
				}
			}
		}
	}

	int queriesWOprocess = 0;
	string key;
	bool show;
	for (std::vector <SQL_pair>::iterator it = myvec.begin(); it != myvec.end(); ++it)
	{
		show = true;
		key = it->first;
		if (files.find(key) == files.end()) {
			queriesWOprocess++;
			if (queriesWOprocess > 5) {
				show = false;
			}
		}
		if (show) {
			stringStream << "&SQL[" << it->first << "]=";
			stringStream << it->second.id << "|";
			stringStream << it->second.user << "|";
			stringStream << it->second.host << "|";
			stringStream << it->second.db << "|";
			stringStream << it->second.commandtype << "|";
			stringStream << it->second.time << "|";
			stringStream << it->second.state << "|";
			string sql = it->second.info;
			if (obfuscatesql) {
				sanitizeSQL(&sql, '\'');
				sanitizeSQL(&sql, '\"');
			}
			encode(&sql);
			stringStream << sql << "|";
			stringStream << it->second.shard ;
		}
	}

    std::vector<ProcPair> sortedProcesses(processes.begin(), processes.end());
    std::sort(sortedProcesses.begin(), sortedProcesses.end(), SortByLoad<ProcPair>());

    int top = 0;
	for (std::vector <ProcPair>::iterator it = sortedProcesses.begin(); it != sortedProcesses.end(); ++it)
	{
		if (it->second.cpuLoad > 0 && top < 10) {
			top++;
			stringStream << "&TOP[" << it->first << "]=";
			stringStream << it->second.name << "|";
			stringStream << it->second.state << "|";
			stringStream << fixed << it->second.cpuLoad;
		}
	}


	post = stringStream.str();
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, serverPostDataURL.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
		if (hideoutput) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerc);	/* Function Pointer "writer" manages the required buffer size */
		}
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
		  fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}
