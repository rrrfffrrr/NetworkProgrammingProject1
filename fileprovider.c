/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "fileprovider.h"

enum EMiddlewareReturn StaticFileProvider(int client, char* data) {
	return MDRET_Continue;
}