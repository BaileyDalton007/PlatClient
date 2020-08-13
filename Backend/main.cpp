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
#include "configParser.h"

Discord* g_Discord;
uintptr_t fovAddr;
uintptr_t handAddr;

int main()
{
	Config config{};
	config = getConfig(config);

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

	if (config.zoomBool == true)
	{
		//Resolve base address of pointer chain
		uintptr_t fovDynamicPtrBaseAddr = moduleBase + 0x0365F7D8;
		uintptr_t handDynamicPtrBaseAddr = moduleBase + 0x036D3C58;

		//Resolve zoom pointer chain
		std::vector<unsigned int> fovOffsets = { 0x0, 0x528, 0x110, 0xE0, 0xB0, 0x120, 0xF0 };
		std::vector<unsigned int> handOffsets = { 0x120, 0xC8, 0x940, 0xE0, 0x8, 0xCE8, 0xE8 };


		fovAddr = mem::FindDMAAddy(hProcess, fovDynamicPtrBaseAddr, fovOffsets);
		handAddr = mem::FindDMAAddy(hProcess, handDynamicPtrBaseAddr, handOffsets);
	}
	std::cout << "Memory Regions found" << std::endl;


	startRPCClientThread();

	float currFov;
	DWORD hideHand = 1;
	DWORD showHand = 0;

	while (true) {
		getCurrClick();
		getCurrRClick();

		updateCPS();
		updateRCPS();


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


