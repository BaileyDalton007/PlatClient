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

int keystrokesInit(IDirect3DDevice9Ex* device, int width, int height);

int drawKeystrokes(IDirect3DDevice9Ex* device, int width, int height, ID3DXFont* font);

int drawW(IDirect3DDevice9Ex* device, int width, int height);
int drawA(IDirect3DDevice9Ex* device, int width, int height);
int drawS(IDirect3DDevice9Ex* device, int width, int height);
int drawD(IDirect3DDevice9Ex* device, int width, int height);