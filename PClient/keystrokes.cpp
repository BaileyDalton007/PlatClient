#include "Paint.h"
#include <math.h>
#include <vector>
#include <winuser.h>
#pragma comment (lib, "User32.lib")
#include "subCount.h"
#include "PClient.h"
#include <tchar.h>
#include "cps.h"

float wPosX;
float wPosY;

Config keyConfig = getConfig(keyConfig);

int configScale = keyConfig.keyStrokesScale; 
float keyScale = float(float(configScale) / 100);


RECT passiveRect = { 64, 0, 128, 64 };
RECT activeRect = { 0, 0, 64, 64 };

LPD3DXSPRITE lmbSprite = NULL;
IDirect3DTexture9* lmbTexture = NULL;

LPD3DXSPRITE rmbSprite = NULL;
IDirect3DTexture9* rmbTexture = NULL;


LPD3DXSPRITE wSprite = NULL;
LPDIRECT3DTEXTURE9 wTexture = NULL;

LPD3DXSPRITE aSprite = NULL;
LPDIRECT3DTEXTURE9 aTexture = NULL;

LPD3DXSPRITE sSprite = NULL;
LPDIRECT3DTEXTURE9 sTexture = NULL;

LPD3DXSPRITE dSprite = NULL;
LPDIRECT3DTEXTURE9 dTexture = NULL;

::D3DXMATRIX mouseButtonScalingMatrix;

D3DXMATRIX keyStrokesScaling;

D3DXVECTOR2 scaling(float(keyScale), keyScale);

int keystrokesInit(IDirect3DDevice9Ex* device, int width, int height)
{

    device ->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    //creates textures
    if (keyConfig.showCPS)
    {
        D3DXCreateTextureFromFile(device, L"assests/lmb.png", &lmbTexture);
        D3DXCreateTextureFromFile(device, L"assests/rmb.png", &rmbTexture);

        D3DXCreateSprite(device, &lmbSprite);
        D3DXCreateSprite(device, &rmbSprite);
    }

    if (keyConfig.showKeystrokes)
    {
        D3DXMatrixTransformation2D(&keyStrokesScaling, NULL, 0.0, &scaling, NULL, NULL, NULL);

        D3DXCreateTextureFromFile(device, L"assests/wkey.png", &wTexture);
        D3DXCreateTextureFromFile(device, L"assests/akey.png", &aTexture);
        D3DXCreateTextureFromFile(device, L"assests/skey.png", &sTexture);
        D3DXCreateTextureFromFile(device, L"assests/dkey.png", &dTexture);

        D3DXCreateSprite(device, &wSprite);
        D3DXCreateSprite(device, &aSprite);
        D3DXCreateSprite(device, &sSprite);
        D3DXCreateSprite(device, &dSprite);
    }

    wPosX = width - keyConfig.keyStrokesX;
    wPosY = keyConfig.keyStrokesY;

    return 0;
}

int drawW(IDirect3DDevice9Ex* device, int width, int height)
{

    wSprite->Begin(NULL);

    if (GetKeyState(0x57) & 0x8000)
    {
        wSprite->Draw(wTexture, &activeRect, NULL, &D3DXVECTOR3(wPosX / keyScale, wPosY / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        wSprite->SetTransform(&keyStrokesScaling);
    }
    else {
        wSprite->Draw(wTexture, &passiveRect, NULL, &D3DXVECTOR3(wPosX/keyScale, wPosY / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        wSprite->SetTransform(&keyStrokesScaling);
    }

    wSprite->End();

    return 0;
}

int drawA(IDirect3DDevice9Ex* device, int width, int height)
{
    aSprite->Begin(NULL);
      
    if (GetKeyState(0x41) & 0x8000)
    {
        aSprite->Draw(aTexture, &activeRect, NULL, &D3DXVECTOR3((wPosX - (70*keyScale)) / keyScale, (wPosY + (70 * keyScale)) / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        aSprite->SetTransform(&keyStrokesScaling);
    }
    else {
        aSprite->Draw(aTexture, &passiveRect, NULL, &D3DXVECTOR3((wPosX - (70 * keyScale)) / keyScale, (wPosY + (70 * keyScale)) / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        aSprite->SetTransform(&keyStrokesScaling);

    }

    aSprite->End();

    return 0;
}

int drawS(IDirect3DDevice9Ex* device, int width, int height)
{
    sSprite->Begin(NULL);

    if (GetKeyState(0x53) & 0x8000)
    {
        sSprite->Draw(sTexture, &activeRect, NULL, &D3DXVECTOR3(wPosX / keyScale, (wPosY + (70 * keyScale)) / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        sSprite->SetTransform(&keyStrokesScaling);

    }
    else {
        sSprite->Draw(sTexture, &passiveRect, NULL, &D3DXVECTOR3(wPosX / keyScale, (wPosY + (70 * keyScale)) / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        sSprite->SetTransform(&keyStrokesScaling);

    }

    sSprite->End();

    return 0;
}

int drawD(IDirect3DDevice9Ex* device, int width, int height)
{
    dSprite->Begin(NULL);

    if (GetKeyState(0x44) & 0x8000)
    {
        dSprite->Draw(dTexture, &activeRect, NULL, &D3DXVECTOR3((wPosX + (70 * keyScale)) / keyScale, (wPosY + (70 * keyScale)) / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        dSprite->SetTransform(&keyStrokesScaling);

    }
    else {
        dSprite->Draw(dTexture, &passiveRect, NULL, &D3DXVECTOR3((wPosX + (70 * keyScale)) / keyScale, (wPosY + (70 * keyScale)) / keyScale, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        dSprite->SetTransform(&keyStrokesScaling);

    }

    dSprite->End();

    return 0;
}

int drawLMB(IDirect3DDevice9Ex* device, int width, int height)
{
    lmbSprite->Begin(NULL);

    float lmbPosX = ((wPosX - 70 * keyScale) / 0.773) / keyScale;
    float lmbPosY = ((wPosY + 140 * keyScale) / 0.510) / keyScale;

    if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
        lmbSprite->Draw(lmbTexture, NULL, NULL, &D3DXVECTOR3(lmbPosX, lmbPosY, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        lmbSprite->SetTransform(&mouseButtonScalingMatrix);

    }
    else {
        lmbSprite->Draw(lmbTexture, NULL, NULL, &D3DXVECTOR3(lmbPosX, lmbPosY, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        lmbSprite->SetTransform(&mouseButtonScalingMatrix);
    }

    lmbSprite->End();

    return 0;
}

int drawRMB(IDirect3DDevice9Ex* device, int width, int height)
{
    rmbSprite->Begin(NULL);

    float lmbPosX = ((wPosX - 70 * keyScale) / 0.773) / keyScale;
    float lmbPosY = ((wPosY + 140 * keyScale) / 0.510) / keyScale;

    if (GetKeyState(VK_RBUTTON) & 0x8000)
    {
        rmbSprite->Draw(rmbTexture, NULL, NULL, &D3DXVECTOR3(lmbPosX + (105 / 0.773) , lmbPosY, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        rmbSprite->SetTransform(&mouseButtonScalingMatrix);
    }
    else {
        rmbSprite->Draw(rmbTexture, NULL, NULL, &D3DXVECTOR3(lmbPosX + (105 / 0.773), lmbPosY, 0.0f), D3DCOLOR_ARGB(0, 255, 255, 255));
        rmbSprite->SetTransform(&mouseButtonScalingMatrix);
    }

    rmbSprite->End();

    return 0;
}

Paint paintThing;

int drawCPS(IDirect3DDevice9Ex* device, int width, int height, ID3DXFont* font)
{
    float cpsPosX = wPosX - (30 * keyScale);
    float rcpsPosX = cpsPosX + (105 * keyScale);

    float cpsPosY = wPosY + (170*keyScale);

    paintThing.drawText((char*)getCPS(), cpsPosX, cpsPosY, 255, 255, 255, 255, font);
    paintThing.drawText((char*)getRCPS(), rcpsPosX, cpsPosY, 255, 255, 255, 255, font);

    return 0;
}

int drawKeystrokes(IDirect3DDevice9Ex* device, int width, int height, ID3DXFont* font)
{
    if (keyConfig.showKeystrokes)
    {
        drawW(device, width, height);
        drawA(device, width, height);
        drawS(device, width, height);
        drawD(device, width, height);
    }

    if (keyConfig.showCPS)
    {
        ::D3DXMatrixScaling(&mouseButtonScalingMatrix, 0.773 * keyScale, 0.510 * keyScale, 1.0f);
        drawLMB(device, width, height);
        drawRMB(device, width, height);

        drawCPS(device, width, height, font);
    }
    return 0;
}
