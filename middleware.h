/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#ifndef __H_MIDDLEWARE_
#define __H_MIDDLEWARE_
#include "packet.h"

#define MAX_MDCALLBACK 128

// middleware return value to control flow
enum EMiddlewareReturn {
	MDRET_Handled,
	MDRET_Continue
};

// middleware callback MDRET (clientFD, data);
typedef enum EMiddlewareReturn (*middleware_callback)(int, char*);

// middleware data and methods
struct SMiddleware {
	middleware_callback callbacks[MAX_MDCALLBACK];
	unsigned int count;
};
void RunMiddleware(struct SMiddleware*, int, char*);
#define MIDDLEWARE(m) struct SMiddleware m; m.count = 0
#define AddMiddleware(m, f) m.callbacks[m.count++] = f

#endif