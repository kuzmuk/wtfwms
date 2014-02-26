/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "wtfwmsd.h"

int start_child() {
	int exit_code;
	if (fork() == 0) {
		char * path_end;
		char buffer[PATH_MAX];
		char worker_path[PATH_MAX];
		if (readlink("/proc/self/exe", buffer, sizeof(buffer)) <=0) {
			return -1;
		}
		path_end = strrchr(buffer, '/');
		if (path_end == NULL) {
			return -1;
		}
		++path_end;
		*path_end = '\0';
		sprintf(worker_path, "%s%s", buffer, "wtfworker");
		execl(worker_path, NULL);
	} else {
		wait(&exit_code);
		if (WIFEXITED(exit_code) && WEXITSTATUS(exit_code) != 42) {
			return 0;
		} else {
			return 1;
		}
	}
}
int killworkers() {
	int exit_code;
	if (fork() == 0) {
		execl("/usr/bin/killall", "/usr/bin/killall", "wtfwroker", NULL);
	} else {
		wait(&exit_code);
		if (WIFEXITED(exit_code) && WEXITSTATUS(exit_code) != 42) {
			return 0;
		} else {
			return 1;
		}
	}
}

int main() {
	while (start_child() > 0) {
		killworkers();
	}
	return 0;
}

