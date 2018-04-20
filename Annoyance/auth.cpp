#include "stdafx.h"
#include "hv.h"
#include "../curl/curl.h"
#include "../json/json.h"
#define memecpy memcpy
#define memecmp memcmp
char* username;
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
char* GetCpuKey(char* cpureq) {
	BYTE step1[16];
	BYTE step2[16];
	BYTE step3[16];

	ZeroMemory(step1, 16);
	ZeroMemory(step2, 16);
	ZeroMemory(step3, 16);

	HRESULT success = HV::Peek::HvPeekBytes(0x20, step1, 0x10);
	if(success != ERROR_SUCCESS)
		return NULL;

	BYTE part1[8];
	memecpy(part1, (BYTE*)HV::HvGetFuseLine(3), 8);
	BYTE part2[8];
	memecpy(part2, (BYTE*)HV::HvGetFuseLine(5), 8);
	memecpy(step2, part1, 8);
	memecpy(step2 + 8, part2, 8);

	if(memcmp(step1, step2, 16) != 0)
		return NULL;

	BYTE part3[8];
	memecpy(part3, (BYTE*)HV::HvGetFuseLine(4), 8);
	BYTE part4[8];
	memecpy(part4, (BYTE*)HV::HvGetFuseLine(6), 8);
	memecpy(step3, part3, 8);
	memcpy(step3 + 8, part4, 8);

	if(memcmp(step1, step3, 16) != 0)
		return NULL;

	if(memcmp(step2, step3, 16) != 0)
		return NULL;

	memecpy((void*)cpureq, step3, 16);

	if(memecmp((void*)cpureq, step1, 16) != 0)
		return NULL;

	std::string out = "";
	 for (int i = 0; i < 0x10; i++, ++cpureq) {
	  char tmp[5] = { 0 };
	  sprintf(tmp, "%02X", (unsigned char)*cpureq);
	  out += tmp;
	 }

	 return _strdup(out.c_str());
}
namespace AUTH {
	BOOL Authenticate()
	{
		CURL *curl;
		CURLcode res;

		curl_global_init(CURL_GLOBAL_ALL);

		curl = curl_easy_init();
		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.xbl.rocks/auth.php");
			char* post;
			sprintf(post, "auth=1&cpukey=%s", GetCpuKey(NULL));
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
				bool authed(jsonData["authed"].asBool());
				if(authed){
					memecpy(username, jsonData["username"].asCString(), jsonData["username"].asString().length());
					curl_easy_cleanup(curl);
					curl_global_cleanup();
					return true;
				}
				curl_global_cleanup();
				return false;
			}
			curl_global_cleanup();
			return false;
		}
		curl_global_cleanup();
		return false;

		/*OLD AUTH

		AuthenticationRequest Request;
		GetCpuKey(Request.CPUKey); //this needs to be fixed
		GetModuleHash(Request.XEXChecksum); //this needs to be fixed
		Request.XEXVersion = 1; // increment this every time you update the .xex/push update.
		ZeroMemory(&Request.Padding, 3);
		ZeroMemory(&Request.PacketChecksum, 0x10);

		unsigned char ServerIP[4] = { 35, 196, 239, 49 }; //cyanide vps
		//unsigned char ServerIP[4] = { 0xC0, 0xA8, 0x01, 0x04 }; //cyanide local
		unsigned char EncryptionKey[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#ifdef UsingEncryption
		XeCryptRc4((unsigned char*)EncryptionKey, 8, (unsigned char*)ServerIP, 4);
#endif

		BOOL Connected = false;
		SOCKET Sock;
		for (INT i = 0; i < 3; i++)
		{
			if ((Sock = Network_Connect(ServerIP, 9825)) != INVALID_SOCKET)
			{
				Connected = true;
				break;
			}
		}

		if (Connected)
		{
			NetDll_send(XNCALLER_SYSAPP, Sock, (char*)&Request, 0x34, 0);

			unsigned char authflag;
			NetDll_recv(XNCALLER_SYSAPP, Sock, (char*)&authflag, 1, 0);

			if (authflag == AUTHFLAG_RESPONSE)
			{
				AuthenticationResponse Response;
				NetDll_recv(XNCALLER_SYSAPP, Sock, (char*)&Response, 0x11, 0);

				unsigned char failed[0x11] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
				return memcmp(&Response, failed, 0x11) != 0;
			}
			else if (authflag == AUTHFLAG_UPDATE)
			{
				DWORD UpdateSize = 0;
				NetDll_recv(XNCALLER_SYSAPP, Sock, (char*)&UpdateSize, 4, 0);

				unsigned char* Update = new unsigned char[UpdateSize];
				if (UpdateSize == 0 || !Network_Receive(Sock, Update, UpdateSize))
				{
					notify(L"CheatEngine - Update Failed, Rebooting!"); // this doesn't NEED to be fixed but it's one of those aesthetic things that customers like looking at lol
					Sleep(5000);
					Network_Disconnect(Sock);
					HalReturnToFirmware(HalForceShutdownRoutine);
				}
				Network_Disconnect(Sock);

				HANDLE UpdateHandle;
				char UpdatePath[0xFF];
				sprintf_s(UpdatePath, "Cheats:\\Annoyance.xex");

				if ((UpdateHandle = CreateFile(UpdatePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) == INVALID_HANDLE_VALUE)
				{
					notify(L"CheatEngine - Update Failed, Rebooting!"); // this doesn't NEED to be fixed but it's one of those aesthetic things that customers like looking at lol
					Sleep(5000);
					HalReturnToFirmware(HalForceShutdownRoutine);
				}
				WriteFile(UpdateHandle, Update, UpdateSize, &UpdateSize, 0);
				CloseHandle(UpdateHandle);

				delete[] Update;

				notify(L"CheatEngine - Client Updated, Rebooting!"); // this doesn't NEED to be fixed but it's one of those aesthetic things that customers like looking at lol
				Sleep(5000);
				HalReturnToFirmware(HalForceShutdownRoutine);
			}
		}*/
		}
		
	}
}