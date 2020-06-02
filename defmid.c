/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "defmid.h"
#include <stdio.h>

enum EMiddlewareReturn PrintToConsole(int client, char* data) {
	printf("\e[93m###DATA###\e[0m\n%s\n\e[93m##########\e[0m\n", data);
	return MDRET_Continue;
}

enum EMiddlewareReturn Response404(int client, char* data) {
	write(client, "HTTP/2 404 Not Found\ncontent-length: 4\ncontent-type: text/html; charset=UTF-8\n\n404 ", 84);
	return MDRET_Handled;
}