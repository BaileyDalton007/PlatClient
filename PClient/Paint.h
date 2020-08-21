//Paint.h
#pragma once

#include <string> //save error
#include <Windows.h>
#include <winuser.h>

#include "d3d9.h"
#include "DirectX/d3dx9.h"

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "../ExternalOverlay/DirectX/d3dx9.lib")
#pragma comment(lib, "d3dx9.lib")


//#include "../ExternalOverlay/DirectX/DxErr.h" //get error from error code
#include "DxErr.h"

//#pragma comment(lib, "../ExternalOverlay/DirectX/dxerr.lib")
#pragma comment(lib, "dxerr.lib")

#pragma comment(lib, "legacy_stdio_definitions.lib")

class Paint {
private:
    IDirect3D9Ex* d3dObject = NULL; //used to create device
    IDirect3DDevice9Ex* d3dDevice = NULL; //contains functions like begin and end scene
    D3DPRESENT_PARAMETERS d3dparams; //parameters for creating device
    HWND targetWnd;
    HWND hWnd;

    int d3D9Init(HWND hWnd);
    

public:
    Paint();
    Paint(HWND hWnd, HWND targetWnd, int width, int height);
    int render();
    int width;
    int height;
    void drawText(char* String, int x, int y, int a, int r, int g, int b, ID3DXFont* font);
    ID3DXFont* mainFont = 0; // font used when displaying text


};
