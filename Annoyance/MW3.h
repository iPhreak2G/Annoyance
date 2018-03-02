#pragma once


namespace MW3{
	DWORD MW3ADDY = 0x83F00000;
	INT increment = 0;
	BOOL found = false;
	INT length = 500000;
	BOOL inGame = true;
	BYTE dump[] = { 0x00 };
	BYTE anti = {0x00};
	BYTE fook = {0x65};

	VOID Enabled(){
		memcpy((BYTE*)dump, (DWORD*)MW3ADDY, length);
		Sleep(1000);
		for (int i = 0; i < length; i++){
            if (dump[i + 1] == 0x6E && dump[i + 2] == 0x64 && dump[i + 3] == 0x72 && dump[i + 4] == 0x6F && dump[i + 5] == 0x75 && dump[i + 6] == 0x6E && dump[i + 7] == 0x64)
            {
                Sleep(1000);
				memcpy((BYTE*)(MW3ADDY + (DWORD)i), (BYTE*)anti, sizeof(anti));
                increment = i;
                inGame = false;
                found = true;
                break;
            }
        }
	}
	VOID Disabled(){
		memcpy((BYTE*)dump, (DWORD*)MW3ADDY, length);
		Sleep(1000);
		for (int i = 0; i < length; i++){
            if (dump[i + 1] == 0x6E && dump[i + 2] == 0x64 && dump[i + 3] == 0x72 && dump[i + 4] == 0x6F && dump[i + 5] == 0x75 && dump[i + 6] == 0x6E && dump[i + 7] == 0x64)
            {
                Sleep(1000);
				memcpy((BYTE*)(MW3ADDY + (DWORD)i), (BYTE*)fook, sizeof(fook));
                break;
            }
        }
	}
	namespace Hook {
		VOID MW3()
		{			
			for(;;){
				if(IsEmpty(dump, length)){
					if(!inGame){
						memcpy((BYTE*)(MW3ADDY + (DWORD)increment), (BYTE*)fook, sizeof(fook));
						inGame = true;
					}
					found = false;
				}else{
					if(found){
						Enabled();
					}
				}
			}
		}
	}	
}
