//overlay.cpp
// ExternalOverlay.cpp : Defines the entry point for the application.
//
#include <Dwmapi.h>

#include "framework.h"
#include "PClient.h"
#include "Paint.h"
#include <windows.h>
#include <WinUser.h>
#include <wingdi.h>
#include <vector>
#include <tchar.h>
#include <iostream>
#include "mem.h"
#include "Discord.h"
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include <Windows.h>
#include "configParser.h"
#include "pointerParser.h"


#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Gdi32.lib")


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR overlayWindowName[100] = L"PlatClient";  // main window class name & The title bar text
LPCSTR targetWindowName = "Minecraft";  // main window class name & The title bar text
HWND targetHWND, overlayHWND;
int width, height;
Paint paint;

// Forward declarations of functions included in this code module:
ATOM                registerClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

//Backend Variables
Discord* g_Discord;
uintptr_t fovAddr;
uintptr_t handAddr;

int currMenu;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    registerClass(hInstance);

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

    targetHWND = FindWindowA(0, targetWindowName);
    if (targetHWND) {
        RECT rect;
        GetWindowRect(targetHWND, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }
    else
        return FALSE;


    // Perform application initialization:
    
    if (!InitInstance(hInstance, SW_SHOW)) {
        return FALSE;
    }
    paint = Paint(overlayHWND, targetHWND, width, height);
    MSG msg;

    float currFov;
    DWORD hideHand = 1;
    DWORD showHand = 0;
    bool zoomed = false;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        RECT rect;
        GetWindowRect(targetHWND, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        MoveWindow(overlayHWND, rect.left, rect.top, width, height, true);

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
    }
    return (int)msg.wParam;

}


//
//  PURPOSE: Registers the window class.
//
ATOM registerClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName = overlayWindowName;
    wcex.lpszClassName = overlayWindowName;
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    overlayHWND = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, overlayWindowName, overlayWindowName, WS_POPUP,
        1, 1, width, height, nullptr, nullptr, hInstance, nullptr);

    if (!overlayHWND) {
        return FALSE;
    }
    SetLayeredWindowAttributes(overlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(overlayHWND, nCmdShow);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;

    switch (message) {
    case WM_COMMAND:
       wmId = LOWORD(wParam);
       wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            break;
        case IDM_EXIT:
            RemoveFontResourceExA("assests/zoeyfont3.tff", FR_PRIVATE, 0);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:

        paint.render();
        break;
    case WM_DESTROY:
        RemoveFontResourceExA("assests/zoeyfont3.tff", FR_PRIVATE, 0);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
