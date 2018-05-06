#include "stdafx.h"

namespace Server{
	namespace
	{
		std::size_t callback(
				const char* in,
				std::size_t size,
				std::size_t num,
				std::string* out)
		{
			const std::size_t totalBytes(size * num);
			out->append(in, totalBytes);
			return totalBytes;
		}
	}
	
	Response_Buffer userData;
	BOOL CommandPending(std::string lastCommand){
		//check if the server is trying to send us a command
		return FALSE;
	}
	Response_Buffer DecipherResponse(Json::Value buffer){
		memecpy(userData.name, buffer["username"].asCString(), buffer["username"].asString().length());
		//ugh
		
	}
	namespace Send{
		HRESULT Command(std::string id, std::string data, Response_Buffer responseBuffer){
			//send command name, command data, do work server sided, respond with string and decipher into struct.
			if(!CommandPending(id)) Recieve::Data(responseBuffer);
			CURL *curl;
			CURLcode res;

			curl_global_init(CURL_GLOBAL_ALL);

			curl = curl_easy_init();
			if(curl){
				curl_easy_setopt(curl, CURLOPT_URL, "https://api.xbl.rocks/auth.php");
				char* post;
				sprintf(post, "command=%s&cpukey=%s", id, AUTH::GetCpuKey(NULL));
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
				curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
				curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				int httpCode;
				std::unique_ptr<std::string> httpData(new std::string());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
				res = curl_easy_perform(curl);
				if(res != CURLE_OK)
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
				if (httpCode == 200){
				Json::Value jsonData;
				Json::Reader jsonReader;
				if(jsonReader.parse(*httpData, jsonData)){
					DecipherResponse(jsonData);
				}
				
				curl_global_cleanup();
				return false;
			}
			curl_global_cleanup();
		}
	}
	namespace Recieve{
		//if command pending, fetch data
		Response_Buffer Data(Response_Buffer responseBuffer){

		}
	}
}
