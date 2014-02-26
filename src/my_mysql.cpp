/*
WTF with my daemon
(c) 2013 Alex Kuzmuk, Andrey Bistriy, Taras Tarasov
*/

#include "my_mysql.h"

std::map <string, sqlProcess> queries;

string q = "SHOW FULL PROCESSLIST";
void getMySQLProcessList() {
	queries.clear();
	if (mysqlShards.size() > 0) {
		std::list<string>::iterator it;
		for (it = mysqlShards.begin(); it != mysqlShards.end(); it++) {
			getProcessesFromShard(*it);
		}
	}
}

void getProcessesFromShard(string shard) {
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;
	int state;
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql,shard.c_str(),db_user.c_str(),db_password.c_str(),NULL,0,0,0);
	if (connection == NULL) {
		printf(mysql_error(&mysql));
		cerr << endl;
		return;
	}
	state = mysql_query(connection, q.c_str());
	if (state !=0) {
		printf(mysql_error(connection));
		cerr << endl;
		return;
	}
	result = mysql_store_result(connection);
	while ( ( row=mysql_fetch_row(result)) != NULL ) {
		if (row[7]) {
			if (q.compare(row[7]) != 0) {
				sqlProcess s;
				s.id = row[0] ? row[0] : "NULL";
				s.shard = shard;
				s.user = row[1] ? row[1] : "NULL";
				s.host = row[2] ? row[2] : "NULL";
				s.db = row[3] ? row[3] : "NULL";
				s.commandtype = row[4] ? row[4] : "NULL";
				s.time = row[5] ? row[5] : "NULL";
				s.state = row[6] ? row[6] : "NULL";
				s.info = row[7] ? row[7] : "NULL";
				queries[row[2]] = s;
			}
		}
	}
	mysql_free_result(result);
	mysql_close(connection);
}
