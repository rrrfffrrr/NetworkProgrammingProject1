/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#ifndef __H_STATICFILEPROVIDER_
#define __H_STATICFILEPROVIDER_

#include "middleware.h"
#include <stdbool.h>

bool InitStaticFileProvider(char*);
enum EMiddlewareReturn StaticFileProvider(int, char*, struct PacketHeader*);

#endif