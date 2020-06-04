/*
* KANG CHAN YEONG(rrrfffrrr@naver.com)
*/
#ifndef __H_HEADER_
#define __H_HEADER_

struct HeaderField { // node
	char* field;
	char* data;
	struct HeaderField* next;
};

struct PacketHeader {
	char* request;
	struct HeaderField* top;
};

struct PacketHeader* NewPacketHeader();
struct PacketHeader* ParsePacketHeader(char*);
void RemovePacketHeader(struct PacketHeader*);
void ClearHeaderField(struct PacketHeader*);
void AddHeaderField(struct PacketHeader* , char* , char* );
void RemoveHeaderField(struct PacketHeader*, char*);
char* BuildHeaderString(struct PacketHeader*);

#endif