/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "fileprovider.h"

bool InitStaticFileProvider(char* path) {
	return true;
}
enum EMiddlewareReturn StaticFileProvider(int client, char* data) {
	return MDRET_Continue;
}