#pragma once
#ifndef CD3D_H
#define CD3D_H

#include <winbase.h>

#define LEFT		(DT_LEFT|DT_NOCLIP)
#define RIGHT		(DT_RIGHT|DT_NOCLIP)
#define CENTER		(DT_CENTER|DT_NOCLIP)

class cD3D
{
private:
	struct D3DTLVERTEX
	{
		float x, y, z, rhw;
		DWORD color;
	};
	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXFONT pFont;

public:
	cD3D()
	{
		pFont = NULL;
	};

	void createDevice(HWND hWnd);
	LPDIRECT3DDEVICE9 getDirectDevice();

	void DrawLine(int startX, int startY, int endX, int endY, DWORD Colour);
	void DrawBox(int x, int y, int Width, int Height, DWORD Colour);
	void DrawBorder(int x, int y, int Width, int Height, DWORD Colour);
	void DrawFilledBox(int x, int y, int Width, int Height, DWORD BorderColour, DWORD FillColour);
	void FillGradient(int x, int y, int Width, int Height, DWORD TopColour, DWORD BottomColour);

	//Functions related to Drawing Text to the screen.
	void CreateFont(int FontSize, char *FontName);
	void PrintText(int x, int y, DWORD Format, DWORD Colour, char *Text);
	int  getTextWidth(char *Text);
};

class cSprite
{
private:
	LPDIRECT3DTEXTURE9 myTexture;
	LPD3DXSPRITE mySprite;
	
	cD3D *myD3D;
	char *myLocation;
	int Width, Height;
public:
	cSprite(cD3D *D3D, char *SpriteLocation, int Width, int Height)
	{
		myLocation = SpriteLocation;
		myD3D = D3D;

		this->Width = Width;
		this->Height = Height;
		if(GetFileAttributes(SpriteLocation) == 0xFFFFFFFF)
		{
			printf("File %s was not found (new cSprite)\n", SpriteLocation);
		}else{
			D3DXCreateTextureFromFileEx(myD3D->getDirectDevice(), myLocation, Width, Height, 
				D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
				0, NULL, NULL, &myTexture);
			D3DXCreateSprite(myD3D->getDirectDevice(), &mySprite);
		}
	}

	int getWidth() { return Width; }
	int getHeight() { return Height; }

	void drawSprite(int x, int y);
	void Release();
};
#endif