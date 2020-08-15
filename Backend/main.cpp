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
#include "configParserBackend.h"
#include "pointerParser.h"

Discord* g_Discord;
uintptr_t fovAddr;
uintptr_t handAddr;
uintptr_t menuAddr;

int currMenu;

int main()
{
	Config config{};
	config = getConfig(config);

	Pointer pointerJson{};
	pointerJson = getPointers(pointerJson);

	if (config.discordRPCBool == true)
	{
		//Discord Rich Presence
		g_Discord->Initialize();
		g_Discord->Update();
	}

	//Get ProcID of the target Process
	static DWORD procId = mem::GetProcId(L"Minecraft.Windows.exe");

	//GetModuleBaseAddress
	static uintptr_t moduleBase = mem::GetModuleBaseAddress(procId, L"Minecraft.Windows.exe");

	//Get Handle to Process
	static HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t menuDynamicPtrBaseAddr = moduleBase + pointerJson.menuBase;
	std::vector<unsigned int> menuOffsets = pointerJson.menuOffsets;
	menuAddr = mem::FindDMAAddy(hProcess, menuDynamicPtrBaseAddr, menuOffsets);


	if (config.zoomBool == true)
	{
		//Resolve base address of pointer chain
		uintptr_t fovDynamicPtrBaseAddr = moduleBase + pointerJson.fovBase;
		uintptr_t handDynamicPtrBaseAddr = moduleBase + pointerJson.handBase;

		//Resolve zoom pointer chain
		std::vector<unsigned int> fovOffsets = pointerJson.fovOffsets;
		std::vector<unsigned int> handOffsets = pointerJson.handOffsets;


		fovAddr = mem::FindDMAAddy(hProcess, fovDynamicPtrBaseAddr, fovOffsets);
		handAddr = mem::FindDMAAddy(hProcess, handDynamicPtrBaseAddr, handOffsets);
	}
	std::cout << "Memory Regions found" << std::endl;


	startRPCClientThread();

	float currFov;
	DWORD hideHand = 1;
	DWORD showHand = 0;

	while (true) {
		
		ReadProcessMemory(hProcess, (BYTE*)menuAddr, &currMenu, sizeof(currMenu), nullptr);

		if (config.zoomBool == true)
		{
			ReadProcessMemory(hProcess, (BYTE*)fovAddr, &currFov, sizeof(currFov), nullptr);
			float oldFov;
			if (GetKeyState(config.zoomKey) & 0x8000)
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
	}

	getchar();
	return 0;
}

const char* getCurrMenu()
{
	if (currMenu == 1) {
		return (const char*)"1";
	}
	else {
		return (const char*)"0";
	}
}


