/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "main.h"

using namespace std;

int activeThreads, a;
bool exitonnextloop;
unsigned char cMacAddr[8];
char macAddress[17];
unsigned int purgeNetworkNamesCacheEveryXRun = 0;
int main(int argc, char *argv[])
{   
    pthread_attr_t attr;
	int rc;
	int exitAfterXReports = 100;
	int fixStoppedProcessesAfter;
	prctl(PR_SET_DUMPABLE, 0);
	int fixHttpdEveryXTimes = 0;
	readconfig();
	exitonnextloop = false;
	initSysCalls();
	activeThreads = 0;
	signal (SIGINT, sigint_handler);

	bzero( (void *)&cMacAddr[0], sizeof(cMacAddr) );
	char iface[7];
	sprintf(iface, "eth0");
	if ( !GetSvrMacAddress(iface) )
	{
	// We failed to get the local host's MAC address
	printf( "Fatal error: Failed to get local host's MAC address\n" );
	}
	sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
	cMacAddr[0], cMacAddr[1], cMacAddr[2],
	cMacAddr[3], cMacAddr[4], cMacAddr[5] );

	purgeNetworkNamesCache();

	
	while (!exitonnextloop) {
	    reset();
		activeApacheProcesses = getProcessesList(false);

		if (activeApacheProcesses >= threshold) {
			if (useModStatus) {
				getApacheStatus();
			}
			getCPUState(&(CPUState[0]));
			fixStoppedProcessesAfter = 60;
            if (pthread_mutex_init(&lock, NULL) != 0) {
                    printf("\n mutex init failed\n");
                    return 1;
            }


			for (unsigned j = 0; j< activeApacheProcesses && j < THREADS; j++) {
					// cout << "main() : creating thread, " << j << endl;
					activeThreads++;
					rc = pthread_attr_init(&attr);
					rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
      				rc = pthread_create(&workers[j], &attr, ApacheHandler, (void *)j);
      				if (rc) {
 	        			cerr << "Error:unable to create thread," << rc << endl;
         				exit(-1);
      				}
			}
			while (activeThreads > 0) {
				fixStoppedProcessesAfter--;
				usleep(100000);
				if (fixStoppedProcessesAfter == 0) {
					if (workersName.size() > 0) {
						fixStoppedProcesses();
					} else {
						exit(42);
					}
				}
				if (fixStoppedProcessesAfter == -50 || fixStoppedProcessesAfter == -100) {
					for (int i = 0; i < THREADS; i++) {
						if (details[i].pid>0 && details[i].done == false && details[i].workerpid > 0) {
							cout << "Kill " << details[i].workerpid << endl;

							if (kill(details[i].workerpid, 0) < 0) {
								if (errno != ESRCH)
									perror("detach: checking sanity");
							} else {
								if (kill(details[i].workerpid, SIGKILL) < 0) {
									if (errno != ESRCH)
										perror("detach: stopping child");
								} else {
									details[i].done = true;
								}
							}

						}
					}
				}
				if (fixStoppedProcessesAfter == -100) {
					cout << "hangs up" << endl;
					exit(42);
				}
				a = 0;
                for (int i = 0; i < THREADS; i++) {
                	if (details[i].pid>0 && details[i].done == false) {
                		a++;
                	}
                	if (a > 0) {
                	}
                }
				if (activeThreads > a) {
					activeThreads = a;
				}
			}
			pthread_mutex_destroy(&lock);
			getCPUState(&(CPUState[1]));
			getProcessesList(true);
			getMySQLProcessList();
			postData();
			exitAfterXReports--;
			if (exitAfterXReports < 1) {
				exit(42);
			}
			usleep(reportDelay);
			fixStoppedProcesses();
		}
		usleep(timeout);
		purgeNetworkNamesCacheEveryXRun++;
		if (purgeNetworkNamesCacheEveryXRun > 100) {
			purgeNetworkNamesCache();
			purgeNetworkNamesCacheEveryXRun = 0;
		}
	}
    return 0;
}

void purgeNetworkNamesCache() {
	networkNamesCache.clear();
	networkNamesCache[16777343] = "localhost";
	networkNamesCache[3306] = "mysql";
	networkNamesCache[11211] = "memcached";
	networkNamesCache[80] = "http";
	networkNamesCache[433] = "https";
}

static int GetSvrMacAddress( char *pIface )
{
	int nSD; // Socket descriptor
	struct ifreq sIfReq; // Interface request
	struct if_nameindex *pIfList; // Ptr to interface name index
	struct if_nameindex *pListSave; // Ptr to interface name index

	pIfList = (struct if_nameindex *)NULL;
	pListSave = (struct if_nameindex *)NULL;
	#ifndef SIOCGIFADDR
	// The kernel does not support the required ioctls
	return( 0 );
	#endif

	nSD = socket( PF_INET, SOCK_STREAM, 0 );
	if ( nSD < 0 )
	{
		printf( "File %s: line %d: Socket failed\n", __FILE__, __LINE__ );
		return( 0 );
	}

	pIfList = pListSave = if_nameindex();
	for ( pIfList; *(char *)pIfList != 0; pIfList++ )
	{
		if ( strcmp(pIfList->if_name, pIface) )
			continue;
		strncpy( sIfReq.ifr_name, pIfList->if_name, IF_NAMESIZE );
		if ( ioctl(nSD, SIOCGIFHWADDR, &sIfReq) != 0 ) {
			printf( "File %s: line %d: Ioctl failed\n", __FILE__, __LINE__ );
			return( 0 );
		}
		memmove( (void *)&cMacAddr[0], (void *)&sIfReq.ifr_ifru.ifru_hwaddr.sa_data[0], 6 );
		break;
	}

	if_freenameindex( pListSave );
	close( nSD );
	return( 1 );
}

