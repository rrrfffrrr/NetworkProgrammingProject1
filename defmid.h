/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#ifndef __H_DEFMID_
#define __H_DEFMID_

#include "middleware.h"

enum EMiddlewareReturn PrintToConsole(int, char*, struct PacketHeader*);
enum EMiddlewareReturn LogRawData(int, char*, struct PacketHeader*);
enum EMiddlewareReturn Response404(int, char*, struct PacketHeader*);

#endif