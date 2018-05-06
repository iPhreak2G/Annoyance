#include "stdafx.h"
#include "hv.h"

extern Server::Response_Buffer userData;
BOOL MenuEnabled = FALSE;
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
		if(Server::Send::Command("auth", "1.0", userData) == ERROR_SUCCESS){
			switch(userData.status){
				case NO_AUTH:
					//not authed
					return FALSE;
				case EXPIRED:
					//authed, expired
					return FALSE;
				case BANNED: 
					//banned
					return FALSE;
				case SUCCESS:
					//authed
					return TRUE;
			}
		}
		
	}

	VOID HookAuthLoop() {
		for(;;){
			if(Authenticate()) MenuEnabled = TRUE;
			else MenuEnabled = FALSE;
		}
	}
}