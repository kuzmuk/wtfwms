/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "apache.h"
#include "config.h"

using namespace std;

apacheDetails details[THREADS];
unsigned int activeApacheProcesses;
std::map <unsigned int, openFile> openFiles;


int writer(char *data, size_t size, size_t nmemb, string *buffer){
	int result = 0;
	if(buffer != NULL) {
		buffer -> append(data, size * nmemb);
		result = size * nmemb;
	}
	return result;
}
void reset() {
	memset(details, 0, sizeof(details));
	for (int i = 0; i < THREADS; i++) {
			for (int j = 0; j < 5; j++) {
				details[i].traces[j].systemCallId = -1;
			}
	}
	openFiles.clear();
}

void getApacheStatus() {
	string buffer;
	CURL *curl;
	curl = curl_easy_init();
	apacheDetails ad;
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, statusURL.c_str());
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0); /* Don't follow anything else than the particular url requested*/
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);	/* Function Pointer "writer" manages the required buffer size */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		int res = curl_easy_perform(curl);
		if (res != 0) {
			printf("Could not get server status\r\n");
			return;
		}
		//cout << "Done CURL" << endl;
		std::istringstream stream(buffer);
		std::string line;
		int i =0;
		bool inTable = false;
		bool rowStarted;
		string row;
		char statusP;
		while (std::getline(stream, line)) {
  			//std::cout << line << std::endl;
			if (i++ == 0) {
				size_t pos = line.find("200 OK");
				if (pos == string::npos) {
					printf("Error: response code is not 200 OK");
					return;
				}
			}
			if (inTable == false) {
				size_t pos = line.find("<table border=\"0\"><tr><th>Srv</th>");
				if (pos != string::npos) {
					inTable = true;
				}
			} else {
				size_t pos = line.find("</table>");
                                if (pos != string::npos) {
                                        inTable = false;
                                } else {
					pos = line.find("<tr><td>");
					if (pos != string::npos) {
						rowStarted = true;
						row = "";
					}
					if (rowStarted == true) {
						row.append(line);
					}
					pos = line.find("</td></tr>");
                                        if (pos != string::npos) {
                                                rowStarted = false;
						char tmp[32];
						row.erase(0, 4);
						row.erase(row.length()-5, 5);
						bool found = true;
						while (found == true) {
							found = false;
							pos = row.find("<td>");
							if (pos != string::npos) {
								found = true;
								row.erase(pos,4);
							}
							pos = row.find("</b>");
							if (pos != string::npos) {
								found = true;
								row.erase(pos,4);
							}
							pos = row.find("<b>");
							if (pos != string::npos) {
								found = true;
								row.erase(pos,3);
							}
							pos = row.find("<td nowrap>");
							if (pos != string::npos) {
								found = true;
								row.erase(pos,11);
							}
							pos = row.find("</td>");
							if (pos != string::npos) {
								found = true;
								row.erase(pos,5);
								row.insert(pos, "\t");
							}
						}
						sscanf(row.c_str(), "%*s\t%d\t%*s\t%s\t%f\t%d\t%*s\t%*s\t%*s\t%*s\t%s\t%s\t\%s\%s", &ad.pid, ad.status, &(ad.cpu),&(ad.duration), ad.ip, ad.vhost, ad.querytype, ad.url);
						if (ad.pid > 0 && ad.status[0] != '_' && skipmeURL.compare(ad.url) != 0) {
							map <pid_t, processInfo>::iterator it = processes.find(ad.pid);
							if (it != processes.end()) {		
								if (it->second.state == 'R' || it->second.state == 'D') {  // skip zombies and stopped processes
									for (int i = 0; i < THREADS; i++) {
										if (details[i].pid == ad.pid) {
											strcpy(details[i].status, ad.status);
											details[i].duration = ad.duration;
											strcpy(details[i].ip, ad.ip);
											strcpy(details[i].vhost, ad.vhost);
											strcpy(details[i].querytype, ad.querytype);
											strcpy(details[i].url, ad.url);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		curl_easy_cleanup(curl);
	}
}
