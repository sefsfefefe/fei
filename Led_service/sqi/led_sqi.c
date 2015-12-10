#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
int main(int argc, char* argv[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	rc = sqlite3_open("led_sqi.db", &db);

	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stderr, "Opened database successfully\n");
	}
	sql = "CREATE TABLE COMPANY("  \
	       "ID INT PRIMARY KEY     NOT NULL," \
	       "NAME           TEXT    NOT NULL," \
	       "AGE            INT     NOT NULL," \
	       "PHONE          INT," \
	       "SEX            TEXT,"\
	       "EMAIL          CHAR(50));";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Table created successfully\n");
	}

	sqlite3_close(db);
	return 0;
}
