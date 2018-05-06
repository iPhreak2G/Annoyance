#pragma once


namespace BO2{
	/** variables **/
	DWORD BO2ADDY = 0x83F00000; //base search address	
	INT increment = 0; //loop
	BOOL found = false; //offset found check
	INT length = 500000; //search length
	BOOL inGame = true; //ingame/null bytes check
	BYTE dump[] = { 0x00 }; //empty bytes cache
	BYTE anti = {0x00}; //null
	BYTE fook = {0x65}; // 'e'
	BYTE realip[4] = {0x00, 0x00, 0x00, 0x00}; //null ip
	BYTE spoofedip[4] = {0x01,0x03,0x03,0x07}; //fake ip
	DWORD entry = 0x8242fb70;

	VOID AntiFreezeConsole(){

	}

	VOID Enabled(){ //patch offset
		memcpy((BYTE*)dump, (DWORD*)BO2ADDY, length); //set cache
		Sleep(1000);
		for (int i = 0; i < length; i++){ //loop through cache and search for 'ndround' bytes
            if (dump[i + 1] == 0x6E && dump[i + 2] == 0x64 && dump[i + 3] == 0x72 && dump[i + 4] == 0x6F && dump[i + 5] == 0x75 && dump[i + 6] == 0x6E && dump[i + 7] == 0x64)
            {
                Sleep(1000);
				memcpy((BYTE*)(BO2ADDY + (DWORD)i), (BYTE*)anti, sizeof(anti)); //disable 'endround' server command
                increment = i; //store loop int
                inGame = false; //bytes are not null
                found = true; //offset found
                break; //stop searching 
            }
        }
	}
	VOID Disabled(){ //restore offset (not used)
		memcpy((BYTE*)dump, (DWORD*)BO2ADDY, length); // set cache
		Sleep(1000);
		for (int i = 0; i < length; i++){ //loop through cache and find offset for 'ndround' bytes
            if (dump[i + 1] == 0x6E && dump[i + 2] == 0x64 && dump[i + 3] == 0x72 && dump[i + 4] == 0x6F && dump[i + 5] == 0x75 && dump[i + 6] == 0x6E && dump[i + 7] == 0x64)
            {
                Sleep(1000);
				memcpy((BYTE*)(BO2ADDY + (DWORD)i), (BYTE*)fook, sizeof(fook)); //set bytes back to origanl value of 'endround'
                break; //stop searching
            }
        }
	}
	namespace Hook {
		VOID BO2()
		{
			//BlackOps2 Retail Challenge Bypass (nops)
			*(DWORD*)0x8259A65C = 0x60000000;
			*(DWORD*)0x82497EB0 = 0x60000000;
			*(DWORD*)0x82497F30 = 0x60000000;
			*(DWORD*)0x82497EE0 = 0x60000000;
			*(DWORD*)0x82497EC8 = 0x60000000;
			*(DWORD*)0x82599680 = 0x60000000;
			*(DWORD*)0x82599670 = 0x60000000;
			*(DWORD*)0x82599628 = 0x60000000;
			*(DWORD*)0x8259964C = 0x60000000;
			*(DWORD*)0x825996AC = 0x60000000;
			*(DWORD*)0x825996B4 = 0x60000000;
			*(DWORD*)0x82599644 = 0x60000000;
			*(DWORD*)0x8259964C = 0x60000000;
			memcpy(realip, (DWORD*)0xC13380E0, 4); // store real ip for later
			for(;;){
				if(IsEmpty(realip, 4)){ //check if bytes are null (ingame)
					memcpy(realip, (DWORD*)0xC13380E0, 4); //set real ip
				}
				if(IsEmpty(dump, length)){
					if(!inGame){
						memcpy((BYTE*)(BO2ADDY + (DWORD)increment), (BYTE*)fook, sizeof(fook)); //reset command to prevent crash
						memcpy((DWORD*)0xC13380E0, realip, 4); //reset ip to prevent crash
						inGame = true; //not in game
					}
					found = false; //reset bool
				}else{
					if(found){ //found offset
						Enabled(); //disable endround server command
						memcpy((DWORD*)0xC13380E0, spoofedip, 4); //set fake ip
					}
				}
			}
		}
	}	
}
