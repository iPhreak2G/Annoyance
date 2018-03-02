#ifndef _DETOUR_H
#define _DETOUR_H


class Detour
{
public:
	bool Hooked;
	void RestoreFunction();
	void* HookFunction(DWORD FuncAddress, DWORD OurDestination);

private:
	int* OrStub;
	int IndexDetour;
	int Address;
	static void GLPR(void);
	static int DetourCount;
	unsigned char OriginalBytes[16];
	static unsigned char DetourSection[0x500];
	static DWORD RelinkGPLR(DWORD Offset, DWORD SaveStubAddress, DWORD OriginalAddress);
	void CopyOriginalInstructions(DWORD Address, DWORD SaveStub, DWORD* StubInstructions);
	void DetourFunction(DWORD Address, void* Destination, void* Stub);
	void PatchInJump(DWORD Address, void* Destination, bool Linked);
};


#endif