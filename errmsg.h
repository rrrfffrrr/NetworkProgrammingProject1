/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#ifndef __H_ERRMSG_
#define __H_ERRMSG_

// message function
#define ErrorMessage(n) fprintf(stderr, "\e[31mCritical\e[0m: %s\n", (n))
#define WarningMessage(n) fprintf(stderr, "\e[33mWarning\e[0m: %s\n", (n))

// messages
// ERRORTYPE_CATEGORY_INFO
#define EMSGCRT_PORT_NOPORT "No port provided." 
#define EMSGCRT_PORT_INVALIDPORT "Invalid port provided." 
#define EMSGCRT_SOCKET_NOTSUPPORT "Cannot open socket." 
#define EMSGCRT_SOCKET_CANNOTBIND "Cannot bind address to socket."
#define EMSGWRN_SOCKET_CANNOTACCEPT "Cannot accept new client."
#define EMSGWRN_PACKET_INVALIDPACKET "Get invalid packet."

// exit codes
#define EMSGRET_PORT 1
#define EMSGRET_SOCKET 2

#endif