/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "debug.h"
#include <stdio.h>

enum EMiddlewareReturn PrintToConsole(int client, char* data) {
	printf("\e[93m###DATA###\e[0m\n%s\n\e[93m##########\e[0m\n", data);
	return MDRET_Continue;
}