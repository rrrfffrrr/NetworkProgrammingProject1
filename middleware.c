/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "middleware.h"
#include <stddef.h>

void RunMiddleware(struct SMiddleware* ware, int client, char* data, struct PacketHeader* header) {
	if (ware == NULL)
		return;

	for(int i = 0; i < ware->count; ++i) {
		switch(ware->callbacks[i](client, data, header)) {
			case MDRET_Handled: return;
			case MDRET_Continue: break;
			default: break;
		}
	}
}