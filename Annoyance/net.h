#pragma once


namespace Server{
	typedef struct _Response_Buffer {
		INT status;
		INT days;
		BOOL lifetime;
		CHAR* name;
	} Response_Buffer, *PResponse_Buffer;
	namespace Send{
		HRESULT Command(std::string id, std::string data[200], Response_Buffer responseBuffer);
	}
}