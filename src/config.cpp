/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "config.h"

string statusURL, skipmeURL, serverPostDataURL, hostname, APIKey, db_user, db_password, skipLogFiles, skipDevNull, skipDevRandom, workersName, skipSockets, skipEventPolls, skipPipes, skipAnonInode;
unsigned int timeout, listenforms, read_limit, reportDelay, threshold;
bool obfuscatesql, fetchdataonread, fetchdataonwrite, useModStatus,resolvenetsockets,resolveunixsockets, hideoutput;
list<string> mysqlShards;
void readconfig() {
    serverPostDataURL = "ht";
	statusURL = "";
	skipmeURL = "";
	mysqlShards.clear();
    timeout = 60 * 1000000; // 1 sec
    listenforms = 100; // wait for syscalls no longer than X ms
    read_limit = 512;
    reportDelay = 0;
    threshold = 0;
	useModStatus = false;
    char buf[50];
    gethostname(buf, 50);
    hostname = buf;
    db_user = "wtfwms";
    db_password = "wtfwms";
    serverPostDataURL.append("tp");
    skipLogFiles = "/var/log/";
    skipDevNull = "/dev/null";
    serverPostDataURL.append("s:");
    skipDevRandom = "/dev/urandom";
    skipSockets = "socket:[";
    skipEventPolls  = "eventpoll:[";
    serverPostDataURL.append("//");
    skipPipes =  "pipe:[";
    skipAnonInode = "anon_inode:[";
    serverPostDataURL.append("r.");
	obfuscatesql = false;
	fetchdataonread = false;
	fetchdataonwrite = true;
    serverPostDataURL.append("wtfwms");
	resolveunixsockets = true;
	resolvenetsockets = true;
	hideoutput = true;
	ifstream fin;
	char ss[500] = "/etc/wtfwms.ini";
    serverPostDataURL.append(".c");
	struct stat sts;
	if (stat(ss, &sts) == -1 && errno == ENOENT) {
		cerr << "Error! Config file /etc/wtfwms.ini not found" << endl;
		exit(-1);
	}
    serverPostDataURL.append("om");
	string param, value;
	fin.open(ss);
	size_t comment, pos2;
    serverPostDataURL.append("/service/");

	while (true) {
		if (!fin.good()) break;

		string lineFromFile;
		getline(fin, lineFromFile);
		comment = lineFromFile.find("#");
		if (comment!=std::string::npos) {
			lineFromFile = lineFromFile.substr(0, comment);
		}
		comment = lineFromFile.find(";");
		if (comment!=std::string::npos) {
			lineFromFile = lineFromFile.substr(0, comment);
		}
		pos2 = lineFromFile.find("=");
		if (pos2!=std::string::npos) {
			while (lineFromFile[pos2-1] == ' ') {
				lineFromFile = lineFromFile.substr(0, pos2-1).append(lineFromFile.substr(pos2));
				pos2 = lineFromFile.find("=");
			}
			while (lineFromFile[pos2+1] == ' ') {
				lineFromFile = lineFromFile.substr(0, pos2+1).append(lineFromFile.substr(pos2+2));
				pos2 = lineFromFile.find("=");
			}
			pos2 = lineFromFile.find("=");
			param = lineFromFile.substr(0, pos2);
			value = lineFromFile.substr(pos2+1);
			while (value[value.size()-1] == ' ') {
				value = value.substr(0,value.size()-1);
			}
			while (param[0] == ' ') {
				param = param.substr(1);
			}
			if (param.compare("apikey") == 0) {
				APIKey = value;
				//cout << "API Key '" << APIKey << "'" << endl;
				if (APIKey.size() != 32) {
					cerr << "Wrong API key" << endl;
					exit(-1);
				}
			} else if (param.compare("statusurl") == 0) {
				statusURL = value;
				skipmeURL = "";
				size_t pos = statusURL.find("://");
				if (pos != string::npos) {
					skipmeURL = statusURL.substr(pos+3);
				} else {
					cerr << "Strange server status URL (" << statusURL << ")" << endl;
				}
				pos = skipmeURL.find("/");
				if (pos != string::npos) {
						skipmeURL = skipmeURL.substr(pos);
				}
				if (value.size() > 1) {
					useModStatus = true;
				}
			} else if (param.compare("listenperiod") == 0) {
				long val = atol(value.c_str());
				if (val > 0) {
					timeout = val * 1000000; // secs
				} else {
					cerr << "Error parsing listenperiod value" << endl;
				}
			} else if (param.compare("delaybetweenreports") == 0) {
				long val = atol(value.c_str());
				if (val > 0) {
					reportDelay = val * 1000000; // secs
				} else {
					cerr << "Error parsing delaybetweenreports value" << endl;
				}
			} else if (param.compare("threshold") == 0) {
				long val = atol(value.c_str());
				if (val > 0) {
					threshold = val; // secs
				} else {
					cerr << "Error parsing threshold value" << endl;
				}
			} else if (param.compare("mysqlhostname") == 0) {
				mysqlShards.push_back(value);
			} else if (param.compare("mysqluser") == 0) {
				db_user = value;
			} else if (param.compare("mysqlpassword") == 0) {
				db_password = value;
			} else if (param.compare("mysqlpassword") == 0) {
				db_password = value;
			} else if (param.compare("skipfilespath") == 0) {
				skipLogFiles = value;
			} else if (param.compare("obfuscatesql") == 0) {
				obfuscatesql = value.compare("yes") == 0;
			} else if (param.compare("fetchdataonread") == 0) {
				fetchdataonread = value.compare("yes") == 0;
			} else if (param.compare("resolvenetsockets") == 0) {
				resolvenetsockets = value.compare("yes") == 0;
			} else if (param.compare("resolveunixsockets") == 0) {
				resolveunixsockets = value.compare("yes") == 0;
			} else if (param.compare("fetchdataonwrite") == 0) {
				fetchdataonwrite = value.compare("yes") == 0;
			} else if (param.compare("hideoutput") == 0) {
				hideoutput = value.compare("yes") == 0;
			} else if (param.compare("workername") == 0) {
				workersName = value;
			} else
			{
				cerr << "Unknown param '" << param << "' in config file" << endl;
			}
		}
	}
	serverPostDataURL.append("report");

	if (!useModStatus && workersName.size() == 0) {
		cerr << "Please specify either server status url or worker name" << endl;
		exit(-1);
	}
	serverPostDataURL.append("server");
	if (threshold < 1) {
		cerr << "Please specify threshold" << endl;
		exit(-1);
	}
	serverPostDataURL.append(".php");
	fin.close();
} 

