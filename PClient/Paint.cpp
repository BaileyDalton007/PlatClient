//Paint.cpp
#include "Paint.h"
#include <math.h>
#include <vector>
#include <winuser.h>
#pragma comment (lib, "User32.lib")
#include "subCount.h"
#include "PClient.h"
#include <tchar.h>
#include "keystrokes.h"
#include "configParser.h"
#include "cps.h"
#include "DevFuncs.h"

//Global Sprites
LPD3DXSPRITE ytSprite = NULL;
IDirect3DTexture9* ytTexture = NULL;

::D3DXMATRIX ytScalingMatrix;

Config paintConfig{};
float scaleYT;

int Paint::d3D9Init(HWND hWnd) {

    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject))) {
        exit(1);
    }

    ZeroMemory(&d3dparams, sizeof(d3dparams));

    d3dparams.BackBufferWidth = width;
    d3dparams.BackBufferHeight = height;
    d3dparams.Windowed = TRUE;
    d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dparams.hDeviceWindow = hWnd;
    d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dparams.EnableAutoDepthStencil = TRUE;
    d3dparams.AutoDepthStencilFormat = D3DFMT_D16;

    HRESULT res = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);

    if (FAILED(res)) {
        /*std::wstring ws(DXGetErrorString(res));
        std::string str(ws.begin(), ws.end());    
        std::wstring ws2(DXGetErrorDescription(res));
        std::string str2(ws2.begin(), ws2.end());
        std::string error = "Error: " + str + " error description: " + str2;
        exit(1);*/
    }

    paintConfig = getConfig(paintConfig);

    float q = float(paintConfig.displayScale);
    scaleYT = q / 10;

    //AddFontResourceExA("zoeyfont.ttf", FR_PRIVATE, 0);
    AddFontResourceExA("assests/zoeyfont3.tff", FR_PRIVATE, 0);


    //D3DXCreateFont(d3dDevice, 25, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Comic Sans", &mainFont);
    D3DXCreateFont(d3dDevice, 25, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Zoey Font 3 Regular", &mainFont);
    
    keystrokesInit(d3dDevice, width, height);
    D3DXCreateTextureFromFile(d3dDevice, L"assests/youtube.png", &ytTexture);
    D3DXCreateSprite(d3dDevice, &ytSprite);

    return 0;

}

Paint::Paint() {};

Paint::Paint(HWND hWnd, HWND targetWnd, int width, int height) {
    this->width = width;
    this->height = height;
    this->targetWnd = targetWnd;
    this->hWnd = hWnd;
    d3D9Init(hWnd);
}

const char* zero = "0";

int Paint::render()
{

    if (d3dDevice == nullptr)
        return 1;
    
    d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
    d3dDevice->BeginScene();
    if (paintConfig.showCPS == 1)
    {
        getCurrClick();
        getCurrRClick();

        updateCPS();
        updateRCPS();
    }
    if (getMenuStatus() == 0)
    {
        if (targetWnd == GetForegroundWindow())
        {
            // Youtube
            const char* ytdata = getYoutubeData();
            if (*ytdata != '\0')
            {
                ::D3DXMatrixScaling(&ytScalingMatrix, scaleYT, 0.4f, 1.0f);
                ytSprite->Begin(NULL);
                ytSprite->Draw(ytTexture, NULL, NULL, &D3DXVECTOR3(float(10) / scaleYT, float((height - 70) / 0.4), 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
                ytSprite->SetTransform(&ytScalingMatrix);
                ytSprite->End();

                drawText((char*)getYoutubeData(), 100 + scaleYT * 80, (height - 55), 255, 237, 90, 255, mainFont);
            }

            //Keystrokes
            drawKeystrokes(d3dDevice, width, height, mainFont);
        }
    }

    d3dDevice->EndScene();
    d3dDevice->PresentEx(0, 0, 0, 0, 0);

    return 0;
}

void Paint::drawText(char* String, int x, int y, int a, int r, int g, int b, ID3DXFont* font)
{
    RECT FontPos;
    FontPos.left = x;
    FontPos.top = y;
    font->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}
