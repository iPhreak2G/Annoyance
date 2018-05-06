// Annoyance.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MW1.h"
#include "WAW.h"
#include "MW2.h"
#include "BO1.h"
#include "MW3.h"
#include "BO2.h"
#include "GO.h"
#include "AW.h"
#include "BO3.h"

BOOL InitializedCheats = false;
BOOL dashLoaded = false;
BOOL Cheats = true;
DWORD (__cdecl *XamGetCurrentTitleID)() = (DWORD (__cdecl *)())ResolveFunction(Module_XAM, 0x1CF);
typedef int(*XamInputGetState_t)(WORD dwUserIndex, DWORD r4, PXINPUT_STATE pState);
XamInputGetState_t XamInputGetStateOriginal;
extern char* username;
extern BOOL MenuEnabled;
VOID Begin(){
	for(;;){
		if (XamGetCurrentTitleID() == DASHBOARD)
		{
			if (!dashLoaded)
			{
				Sleep(5000);
				Cheats = MenuEnabled;
				LPWSTR ptr;
				char text[] = "";
				switch(Cheats){
				case true:
					
					sprintf(text, "Annoyance - Authed, Welcome %s", username);
					wchar_t wtext[20];
					mbstowcs(wtext, text, strlen(text)+1);
					ptr = wtext;
					notify(ptr);
				case false:
					notify(L"Annoyance - Not Authed");
				}
				dashLoaded = true;
				InitializedCheats = false;
			}			
		}
		else
		{
			if (Cheats)
			{
				switch(XamGetCurrentTitleID()){
					case COD_MW3:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)MW3::Hook::MW3);					
							//notify(L"Annoyance - MW3 Hooked!");
							InitializedCheats = true;
						}
					}
					case COD_BLACK_OPS_2:{
						if (!InitializedCheats){ 
							Sleep(5000);
							Thread((LPTHREAD_START_ROUTINE)BO2::Hook::BO2);					
							notify(L"Annoyance - BO2 Hooked!");
							InitializedCheats = true;
						}
					}
					case COD_MW:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)MW1::Hook::MW1);					
							notify(L"Annoyance - MW1 Disabled!");
							InitializedCheats = true;
						}
					}
					case COD_MW2:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)MW2::Hook::MW2);					
							notify(L"Annoyance - MW2 Disabled!");
							InitializedCheats = true;
						}
					}
					case COD_BO1:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)BO1::Hook::BO1);					
							notify(L"Annoyance - BO1 Disabled!");
							InitializedCheats = true;
						}
					}
					case COD_BLACK_OPS_3:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)BO3::Hook::BO3);					
							notify(L"Annoyance - BO3 Disabled!");
							InitializedCheats = true;
						}
					}
					case COD_ADVANCED_WARFARE:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)AW::Hook::AW);					
							notify(L"Annoyance - AW Disabled!");
							InitializedCheats = true;
						}
					}
					case COD_GHOSTS:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)GO::Hook::GO);					
							notify(L"Annoyance - GO Disabled!");
							InitializedCheats = true;
						}
					}
					case COD_WAW:{
						if (!InitializedCheats){ 
							Sleep(5000);
							//Thread((LPTHREAD_START_ROUTINE)WAW::Hook::WAW);					
							notify(L"Annoyance - WAW Disabled!");
							InitializedCheats = true;
						}
					}
				}					
			}
		}
	}
}

BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			Thread((LPTHREAD_START_ROUTINE)AUTH::HookAuthLoop());
			Thread((LPTHREAD_START_ROUTINE)Begin);
			break;
		case DLL_PROCESS_DETACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	} return TRUE;
}