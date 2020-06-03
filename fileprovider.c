/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "fileprovider.h"
#include "packet.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define PATH_MAX 4096
static char PWD[PATH_MAX];

// Init public file path to sandbox it
bool InitStaticFileProvider(char* path) {
	if (path == NULL)
		return false;
	// if path is relative, add current working directory to PWD
	PWD[0] = '\0';
	if (path[0] != '/') {
		getcwd(PWD, PATH_MAX);
		strcat(PWD, "/");
	}
	// Add path to it
	strcat(PWD, path);

	// check PWD is directory
	struct stat pstat;
	stat(PWD, &pstat);
	if (S_ISDIR(pstat.st_mode) == 0) { // Fail to initialize when path is not directory
		return false;
	}
	return true;
}
enum EMiddlewareReturn StaticFileProvider(int client, char* data) {
	// only support GET METHOD
	if (data[0] == 'G' && data[1] == 'E' && data[2] == 'T') {
		// copy current working directory to path
		char path[PATH_MAX];
		size_t len = strlen(PWD);
		memcpy(path, PWD, sizeof(char) * len);
		path[len] = '\0';

		// Get URI
		char* uriStart, * uriEnd;
		uriStart = &data[4];
		for(char* cursor = uriStart;;cursor++) {
			if (*cursor == ' ' || *cursor == '\n' || *cursor == '	') { // got end of uri
				uriEnd = cursor;
				break;
			}
			if (*cursor == '\0') // end of file (no uri)
				return MDRET_Continue;
		}

		// copy uri to path to complete target path
		size_t len2 = uriEnd - uriStart;
		memcpy(&path[len], uriStart, len2);
		path[len + len2] = '\0';

		struct stat pstat;
		stat(path, &pstat);
		char packet[PACKET_SIZE];
		packet[0] = '\0';
		char body[PACKET_SIZE];
		body[0] = '\0';
		if (S_ISDIR(pstat.st_mode) != 0) { // Send file list when target is directory

			// Fill body part
			sprintf(body, "<!DOCTYPE html><head></head><body><ul>");

			// Iterate dirent and fill list to body without hidden files
			struct dirent* eDir;
			DIR* pDir = opendir(path);
			if (pDir == NULL) {
				return MDRET_Continue;
			}

			while((eDir = readdir(pDir)) != NULL) {
				if (eDir->d_name[0] != '.') {
					strcat(body, "<li><a href=\"");
					strcat(body, eDir->d_name);
					strcat(body, "\">");
					strcat(body, eDir->d_name);
					strcat(body, "</a></li>");
				}
			}
			closedir(pDir);

			// End body and fill packet with header and body
			strcat(body, "</ul></body>");
			sprintf(packet, "HTTP/2 200 OK\ncontent-length: %ld\ncontent-type: text/html; charset=UTF-8\nserver: NPP/1.0\n\n%s", strlen(body), body);

			// Send packet and stop middlewares
			write(client, packet, strlen(packet));
			return MDRET_Handled;
		} else if (S_ISREG(pstat.st_mode) != 0) { // Send file when it's regular file
			// Select content type
			char contentType[16];
			sprintf(contentType, "text/html"); // hardcoded for test

			// Read file and save to body buffer
			int fd = open(path, O_RDONLY);
			if (fd < 0)
				return MDRET_Continue;
			size_t bSize = read(fd, body, PACKET_SIZE);
			close(fd);

			// Fill packet
			sprintf(packet, "HTTP/2 200 OK\ncontent-length: %ld\ncontent-type: %s; charset=UTF-8\nserver: NPP/1.0\n\n%s", bSize, contentType, body);

			// Send packet and stop middlewares
			write(client, packet, strlen(packet));
			return MDRET_Handled;
		}
	}

	return MDRET_Continue;
}