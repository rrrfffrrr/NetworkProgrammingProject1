/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#ifndef __H_ERRMSG_
#define __H_ERRMSG_

// message function
#define ErrorMessage(n) fprintf(stderr, "\e[31mCritical\e[0m: "); fprintf(stderr, (n))
#define ErrorMessage1(n, a) fprintf(stderr, "\e[31mCritical\e[0m: "); fprintf(stderr, (n), (a))
#define WarningMessage(n) fprintf(stderr, "\e[33mWarning\e[0m: "); fprintf(stderr, (n))
#define WarningMessage1(n, a) fprintf(stderr, "\e[33mWarning\e[0m: "); fprintf(stderr, (n), (a))

// messages
// ERRORTYPE_CATEGORY_INFO
#define EMSGCRT_PORT_NOPORT "No port provided.\n" 
#define EMSGCRT_PORT_INVALIDPORT "Invalid port provided.\n" 
#define EMSGCRT_SOCKET_NOTSUPPORT "Cannot open socket.\n" 
#define EMSGCRT_SOCKET_CANNOTBIND "Cannot bind address to socket(%d).\n"
#define EMSGWRN_SOCKET_CANNOTACCEPT "Cannot accept new client.\n"
#define EMSGWRN_PACKET_INVALIDPACKET "Get invalid packet.\n"

// exit codes
#define EMSGRET_PORT 1
#define EMSGRET_SOCKET 2

#endif