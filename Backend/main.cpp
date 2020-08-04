#pragma once
#include <iostream>
#include <vector>
#include "mem.h"
#include "Discord.h"
#include "main.h"
#include "client.h"
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include <Windows.h>
#include <thread>
#include "cps.h"


Discord* g_Discord;

int main()
{
	//Discord Rich Presence
	g_Discord->Initialize();
	g_Discord->Update();

	//Get ProcID of the target Process
	static DWORD procId = mem::GetProcId(L"Minecraft.Windows.exe");

	//GetModuleBaseAddress
	static uintptr_t moduleBase = mem::GetModuleBaseAddress(procId, L"Minecraft.Windows.exe");

	//Get Handle to Process
	static HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//Resolve base address of pointer chain
	uintptr_t fovDynamicPtrBaseAddr = moduleBase + 0x036AB6B0;
	uintptr_t handDynamicPtrBaseAddr = moduleBase + 0x036D3C58;
	//uintptr_t serverDynamicPtrBaseAddr = moduleBase + 0x036E4528;


	//uintptr_t ignDynamicPtrBaseAddr = moduleBase + 0x036D6CE0;

	//std::cout << "DynamicPtrBaseAddr: 0x" << std::hex << fovDynamicPtrBaseAddr << std::endl;

	//Resolve our fov pointer chain
	std::vector<unsigned int> fovOffsets = {0x98, 0xC8, 0x10, 0xF58, 0xB0, 0x120, 0xF0};
	std::vector<unsigned int> handOffsets = { 0x120, 0xC8, 0x940, 0xE0, 0x8, 0xCE8, 0xE8 };
	//std::vector<unsigned int> serverOffsets = { 0xD8, 0x10, 0x28, 0x100, 0x40, 0x370, 0x0 };


	uintptr_t fovAddr = mem::FindDMAAddy(hProcess, fovDynamicPtrBaseAddr, fovOffsets);
	uintptr_t handAddr = mem::FindDMAAddy(hProcess, handDynamicPtrBaseAddr, handOffsets);
	///uintptr_t serverAddr = mem::FindDMAAddy(hProcess, serverDynamicPtrBaseAddr, serverOffsets);


	std::cout << "Memory Regions found" << std::endl;

	//std::vector<unsigned int> ignOffsets = {0x98, 0x38, 0x28, 0x20, 0x8, 0x8, 0x70};

	//uintptr_t ignAddr = mem::FindDMAAddy(hProcess, ignDynamicPtrBaseAddr, ignOffsets);

	//std::string ign;
	//ReadProcessMemory(hProcess, (BYTE*)ignAddr, &ign, sizeof(ign), nullptr);
	//std::cout << ign << std::endl;
	//std::cout << "ignAddr: 0x" << std::hex << ignAddr << std::endl;

	

	//Read fov value
	//float fovValue = 0;
	//ReadProcessMemory(hProcess, (BYTE*)fovAddr, &fovValue, sizeof(fovValue), nullptr);
	//std::cout << "FOV: " << std::dec << fovValue << std::endl;


	//Write to it
	//float newFov= 30;
	//WriteProcessMemory(hProcess, (BYTE*)fovAddr, &newFov, sizeof(newFov), nullptr);

	//Read out again
	//ReadProcessMemory(hProcess, (BYTE*)fovAddr, &fovValue, sizeof(fovValue), nullptr);
	//std::cout << "FOV2: " << std::dec << fovValue << std::endl;

	startRPCClientThread();

	float currFov;
	DWORD hideHand = 1;
	DWORD showHand = 0;
	//int serverName;

	while (true) {
		getCurrClick();
		getCurrRClick();

		updateCPS();
		updateRCPS();


		//std::cout << updateCPS() << std::endl;
		ReadProcessMemory(hProcess, (BYTE*)fovAddr, &currFov, sizeof(currFov), nullptr);
		//ReadProcessMemory(hProcess, (BYTE*)serverAddr, &serverName, sizeof(serverName), nullptr);
		//std::cout << serverName << std::endl;

		float oldFov;
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			if (currFov != 30) 
			{
				oldFov = currFov;
				float newFov = 30;
				
				WriteProcessMemory(hProcess, (BYTE*)fovAddr, &newFov, sizeof(newFov), nullptr);
				WriteProcessMemory(hProcess, (BYTE*)handAddr, &hideHand, sizeof(hideHand), nullptr);

			}

		}
		else {
			if (currFov == 30) {
				WriteProcessMemory(hProcess, (BYTE*)fovAddr, &oldFov, sizeof(oldFov), nullptr);
				WriteProcessMemory(hProcess, (BYTE*)handAddr, &showHand, sizeof(showHand), nullptr);

			}

		}
	}

	getchar();
	return 0;
}



