/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
* Todo: static file provider
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>

#include "errmsg.h"
#include "header.h"
#include "middleware.h"
#include "defmid.h"
#include "fileprovider.h"

#define PACKET_SIZE 131072

const int MAX_PORT = 65535;
int port = 80;

int backlog = 5;

int main(int argc, char *argv[]) {
	// Parse argument and get port
	if (argc < 2) {
		ErrorMessage(EMSGCRT_PORT_NOPORT);
		exit(EMSGRET_PORT);
	}
	port = atoi(argv[1]);
	if (port > MAX_PORT || port < 0) {
		ErrorMessage(EMSGCRT_PORT_INVALIDPORT);
		exit(EMSGRET_PORT);
	}

	// Initialize middlewares
	MIDDLEWARE(middlewares);
	// Add logger
	AddMiddleware(middlewares, PrintToConsole);
	AddMiddleware(middlewares, LogRawData);

	// Add static file provider
	InitStaticFileProvider("public/");
	AddMiddleware(middlewares, StaticFileProvider);

	// Last middleware must be 404 response
	AddMiddleware(middlewares, Response404);

	// Make a tcp socket
	// Socket created with TCP/IPv4.
	int server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server < 0) {
		ErrorMessage(EMSGCRT_SOCKET_NOTSUPPORT);
		exit(EMSGRET_SOCKET);
	}
	
	// Initialize address structure to bind socket.
	struct sockaddr_in socketAddr;
	bzero((char*)&socketAddr, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;				// IPv4
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// ip address (INADDR_ANY mean selected by system)
	socketAddr.sin_port = htons(port);				// port

	// Bind address to socket
	if (bind(server, (struct sockaddr*)&socketAddr, sizeof(socketAddr)) < 0) {
		ErrorMessage1(EMSGCRT_SOCKET_CANNOTBIND, port);
		exit(EMSGRET_SOCKET);
	}

	// Start server
	listen(server, backlog);

	// Infinite loop
	while(true) {
		// Initialize client address structure
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		// Accept next client
		int client = accept(server, (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (client < 0) {
			WarningMessage(EMSGWRN_SOCKET_CANNOTACCEPT);
			continue;
		}

		// Get packet
		char buf[PACKET_SIZE];
		bzero(buf, PACKET_SIZE);
		int n = read(client, buf, PACKET_SIZE);
		if (n < 0) {
			WarningMessage(EMSGWRN_PACKET_INVALIDPACKET);
			close(client);
			continue;
		}

		struct PacketHeader* header = ParsePacketHeader(buf);

		// Run middleware to process data
		RunMiddleware(&middlewares, client, buf, header);

		RemovePacketHeader(header);

		// Cleanup
		close(client);
	}

	// Clean
	close(server);
	return 0;
}