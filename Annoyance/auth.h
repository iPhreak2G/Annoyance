#pragma once

#define NO_AUTH 1
#define EXPIRED 2
#define BANNED 3
#define SUCCESS 4

namespace AUTH{
	char* GetCpuKey(char* cpureq);
	BOOL Authenticate();
	VOID HookAuthLoop();
}