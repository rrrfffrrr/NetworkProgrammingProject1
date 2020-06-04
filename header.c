/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF 512

struct PacketHeader* NewPacketHeader() {
	struct PacketHeader* header = (struct PacketHeader*)malloc(sizeof(struct PacketHeader));
	header->request = NULL;
	header->top = NULL;
	return header;
}
struct PacketHeader* ParsePacketHeader(char* request) {
	struct PacketHeader* header = NewPacketHeader();
	// get request line
	{
		char* begin = request;
		while(*request != '\n' && *request != '\0') {
			request++;
		}
		size_t len = sizeof(char)*(request-begin);
		header->request = (char*)malloc(len + sizeof(char));
		memcpy(header->request, begin, len);
		header->request[len] = '\0';
		request++;
		while(*request == 13 || *request == '\n' || *request == ' ' || *request == '	')
			request++;
	}
	// parsing headers
	{
		char fieldBuf[MAXBUF];
		char dataBuf[MAXBUF];
		// header:data\n
		char* end = request + strlen(request) + 1;
		for(char* begin = request;request < end;) {
			// copy header
			while(*request!=':')
				request++;
			memcpy(fieldBuf, begin, request-begin);
			fieldBuf[request-begin] = '\0';
			while(*request == 13 || *request == '\n' || *request == ' ' || *request == '	')
				request++;
			begin = ++request;

			// copy data
			while(*request!='\n' && *request!='\0')
				request++;
			memcpy(dataBuf, begin, request-begin);
			dataBuf[request-begin] = '\0';

			// Add node
			AddHeaderField(header, fieldBuf, dataBuf);
			while(*request == 13 || *request == '\n' || *request == ' ' || *request == '	')
				request++;
			begin = ++request;
		}
	}

	return header;
}
void RemovePacketHeader(struct PacketHeader* header) {
	if (header == NULL)
		return;
	ClearHeaderField(header);
	if (header->request != NULL)
		free(header->request);
	free(header);
}


void AddHeaderField(struct PacketHeader* header, char* field, char* data) {
	// allocated new node
	struct HeaderField* new_node = (struct HeaderField*)malloc(sizeof(struct HeaderField));
	new_node->next = NULL;

	// copy field name
	size_t len = sizeof(char)*(strlen(field)+1);
	new_node->field = (char*)malloc(len);
	memcpy(new_node->field, field, len);

	// copy field data
	len = sizeof(char)*(strlen(data)+1);
	new_node->data = (char*)malloc(sizeof(char)*(strlen(data)+1));
	memcpy(new_node->data, data, len);

	// set top if top is null
	if (header->top == NULL) {
		header->top = new_node;
		return;
	}

	// find last node
	struct HeaderField* prev = header->top;
	while(prev->next != NULL) {
		prev = prev->next;
	}

	// append with new node
	prev->next = new_node;
}

void RemoveHeaderFieldData(struct HeaderField* data) {
	if (data == NULL)
		return;

	if (data->field != NULL)
		free(data->field);
	if (data->data != NULL)
		free(data->data);
	free(data);
}

void RemoveHeaderField(struct PacketHeader* header, char* field) {
	// No node exists
	if (header->top == NULL)
		return;

	struct HeaderField* node = header->top;

	// Remove top
	if (strcmp(header->top->field, field) == 0) {
		header->top = node->next;
		RemoveHeaderFieldData(node);
		return;
	}


	// find field
	while(node->next != NULL) {
		if (strcmp(field, node->next->field) == 0)
			break;
	}

	// no node found, exit
	if (node->next == NULL)
		return;

	// Remove it
	struct HeaderField* target = node->next;
	node->next = target->next;
	RemoveHeaderFieldData(target);
}

void ClearHeaderField(struct PacketHeader* header) {
	struct HeaderField* node = header->top;
	struct HeaderField* next;

	// Remove all nodes
	while(node != NULL) {
		next = node->next;
		RemoveHeaderFieldData(node);
		node = next;
	}

	header->top = NULL;
}

char* BuildHeaderString(struct PacketHeader* header) {
	if (header == NULL || header->top == NULL)
		return NULL;

	struct HeaderField* node = header->top;
	size_t len = 1; // string size start with 1 because of \0
	// build string size
	while(node != NULL) {
		// field + : + data + \n
		len += strlen(node->field) + 1 + strlen(node->data) + 1;
		node = node->next;
	}

	node = header->top;
	char* str = (char*)malloc(sizeof(char) * len);

	// fill string with data
	while(node != NULL) {

	}

	return str;
}