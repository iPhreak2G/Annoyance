#pragma once


namespace BO1{
	DWORD BO1ADDY = 0x83F00000; 
	INT increment = 0;
	BOOL found = false;
	INT length = 500000;
	BOOL inGame = true;
	BYTE dump[] = { 0x00 };
	BYTE anti = {0x00};
	BYTE fook = {0x65};
	BYTE realip[4] = {0x00, 0x00, 0x00, 0x00};
	BYTE spoofedip[4] = {0x01,0x03,0x03,0x07};
	VOID Enabled(){
		memcpy((BYTE*)dump, (DWORD*)BO1ADDY, length);
		Sleep(1000);
		for (int i = 0; i < length; i++){
            if (dump[i + 1] == 0x6E && dump[i + 2] == 0x64 && dump[i + 3] == 0x72 && dump[i + 4] == 0x6F && dump[i + 5] == 0x75 && dump[i + 6] == 0x6E && dump[i + 7] == 0x64)
            {
                Sleep(1000);
				memcpy((BYTE*)(BO1ADDY + (DWORD)i), (BYTE*)anti, sizeof(anti));
                increment = i;
                inGame = false;
                found = true;
                break;
            }
        }
	}
	VOID Disabled(){
		memcpy((BYTE*)dump, (DWORD*)BO1ADDY, length);
		Sleep(1000);
		for (int i = 0; i < length; i++){
            if (dump[i + 1] == 0x6E && dump[i + 2] == 0x64 && dump[i + 3] == 0x72 && dump[i + 4] == 0x6F && dump[i + 5] == 0x75 && dump[i + 6] == 0x6E && dump[i + 7] == 0x64)
            {
                Sleep(1000);
				memcpy((BYTE*)(BO1ADDY + (DWORD)i), (BYTE*)fook, sizeof(fook));
                break;
            }
        }
	}
	namespace Hook {
		VOID BO1()
		{
			memcpy(realip, (DWORD*)0xC13380E0, 4);
			for(;;){
				if(IsEmpty(dump, length)){
					if(!inGame){
						memcpy((BYTE*)(BO1ADDY + (DWORD)increment), (BYTE*)fook, sizeof(fook));
						memcpy((DWORD*)0xC13380E0, realip, 4);
						inGame = true;
					}
					found = false;
				}else{
					if(found){
						Enabled();
						memcpy((DWORD*)0xC13380E0, spoofedip, 4);
					}
				}
			}
		}
	}	
}
