
#include <iostream>
#include <vector>
#include "mem.h"
#include "Discord.h"
#include "main.h"
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include <Windows.h>
#include "configParserBackend.h"
#include "pointerParser.h"

Discord* g_Discord;
uintptr_t fovAddr;
uintptr_t handAddr;

int currMenu;

int main()
{
	HideConsole();
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
	static DWORD overlayPID = mem::GetProcId(L"ExternalOverlay.exe");


	//GetModuleBaseAddress
	static uintptr_t moduleBase = mem::GetModuleBaseAddress(procId, L"Minecraft.Windows.exe");

	//Get Handle to Process
	static HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

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


	float currFov;
	DWORD hideHand = 1;
	DWORD showHand = 0;
	bool zoomed = false;

	while (true) {

		if (config.zoomBool == true)
		{
			ReadProcessMemory(hProcess, (BYTE*)fovAddr, &currFov, sizeof(currFov), nullptr);
			float oldFov;
			if (GetKeyState(config.zoomKey) & 0x8000)
			{

				if (currFov != 30)
				{
					if (!zoomed)
					{
						oldFov = currFov;
						float newFov = 30;
						WriteProcessMemory(hProcess, (BYTE*)fovAddr, &newFov, sizeof(newFov), nullptr);
						WriteProcessMemory(hProcess, (BYTE*)handAddr, &hideHand, sizeof(hideHand), nullptr);
						zoomed = true;
					}

				}

			}
			else {
				if (currFov == 30) {
					WriteProcessMemory(hProcess, (BYTE*)fovAddr, &oldFov, sizeof(oldFov), nullptr);
					WriteProcessMemory(hProcess, (BYTE*)handAddr, &showHand, sizeof(showHand), nullptr);
					if (zoomed) { zoomed = false; }

				}
			}
		}
		Sleep(1);
		if (!mem::IsProcessRunning(overlayPID))
		{
			break;
		}
	}

	return 0;
}

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}
