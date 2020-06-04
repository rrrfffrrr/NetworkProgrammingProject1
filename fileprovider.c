/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "fileprovider.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define PACKET_SIZE 524288

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

void SelectContentType(char* path, char* dest) {
	size_t len = strlen(path);
	char buf[8];
	char* pos = strchr(path + len - 5, '.');

	// No extension
	if (pos == NULL) {
		sprintf(dest, "text/plain; charset=UTF-8");
		return;
	}
	strcpy(buf, pos+1);
	// jpg
	if (strcmp(buf, "jpg") == 0 || strcmp(buf, "jpeg") == 0)
		sprintf(dest, "image/jpeg");
	else if (strcmp(buf, "gif") == 0)
		sprintf(dest, "image/gif");
	else if (strcmp(buf, "mp3") == 0)
		sprintf(dest, "audio/mpeg");
	else if (strcmp(buf, "pdf") == 0)
		sprintf(dest, "application/pdf");
	else if (strcmp(buf, "html") == 0) // html
		sprintf(dest, "text/html; charset=UTF-8");
	else
		sprintf(dest, "text/plain; charset=UTF-8");
}

enum EMiddlewareReturn StaticFileProvider(int client, char* data, struct PacketHeader* header) {
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
		char body[PACKET_SIZE];
		// Initialize char array
		memset(packet, '\0', 1);
		memset(body, '\0', 1);
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
			char contentType[32];
			SelectContentType(path, contentType);

			// Read file and save to body buffer
			int fd = open(path, O_RDONLY);
			if (fd < 0)
				return MDRET_Continue;
			size_t bSize = read(fd, body, PACKET_SIZE);
			close(fd);

			// Fill packet
			int writen = sprintf(packet, "HTTP/2 200 OK\ncontent-length: %ld\ncontent-type: %s\nserver: NPP/1.0\n\n", bSize, contentType);
			memcpy(packet + writen, body, bSize);

			// Send packet and stop middlewares
			write(client, packet, writen + bSize);
			return MDRET_Handled;
		}
	}

	return MDRET_Continue;
}