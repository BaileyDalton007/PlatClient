#include <iostream>
#include "mem.h"
#include <string>
#include "configParser.h"
#include "pointerParser.h"

float currFov;
DWORD hideHand = 1;
DWORD showHand = 0;
bool zoomed = false;


void zoomLoop(int key, static HANDLE proc, uintptr_t fov, uintptr_t hand)
{
    ReadProcessMemory(proc, (BYTE*)fov, &currFov, sizeof(currFov), nullptr);
    float oldFov;
    if (GetKeyState(key) & 0x8000)
    {

        if (currFov != 30)
        {
            if (!zoomed)
            {
                oldFov = currFov;
                float newFov = 30;
                WriteProcessMemory(proc, (BYTE*)fov, &newFov, sizeof(newFov), nullptr);
                WriteProcessMemory(proc, (BYTE*)hand, &hideHand, sizeof(hideHand), nullptr);
                zoomed = true;
            }

        }

    }
    else {
        if (currFov == 30) {
            WriteProcessMemory(proc, (BYTE*)fov, &oldFov, sizeof(oldFov), nullptr);
            WriteProcessMemory(proc, (BYTE*)hand, &showHand, sizeof(showHand), nullptr);
            if (zoomed) { zoomed = false; }

        }
    }
}