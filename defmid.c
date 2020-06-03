/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "defmid.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

enum EMiddlewareReturn PrintToConsole(int client, char* data) {
	printf("\e[93m###DATA###\e[0m\n%s\n\e[93m##########\e[0m\n", data);
	return MDRET_Continue;
}

static int logfd = -1;
enum EMiddlewareReturn LogLawData(int client, char* data) {
	if (logfd == -1) {
		time_t now;
		time(&now);
		char logName[128];
		sprintf(logName, "%s.log", ctime(&now));
		logfd = creat(logName ,0666);
	}
	write(logfd, data, strlen(data));
	write(logfd, "\n", 1);
	return MDRET_Continue;
}

enum EMiddlewareReturn Response404(int client, char* data) {
	write(client, "HTTP/2 404 Not Found\ncontent-length: 4\ncontent-type: text/html; charset=UTF-8\n\n404 ", 84);
	return MDRET_Handled;
}