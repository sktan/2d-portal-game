#include "stdafx.h"
#include "cD3D.h"

void cD3D::createDevice(HWND hWnd)
{
	//Create our Direct3D Interface
    LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
	//Ceate a struct to hold various device information
    D3DPRESENT_PARAMETERS d3dpp;

    //Clear out the struct for use
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE; // Make sure the program is Windowed.
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //Discard all frames.
    d3dpp.hDeviceWindow = hWnd; //Set our form to use DirectXD

    //Create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->pDevice);
}

LPDIRECT3DDEVICE9 cD3D::getDirectDevice()
{
	return this->pDevice;
}

void cD3D::CreateFont(int FontSize, char *FontName)
{
	if(isValid(this->pFont))
	{
		this->pFont->Release();
		this->pFont = NULL;
	}
	D3DXCreateFont(this->pDevice, FontSize, NULL, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FontName, &pFont);
}

void cD3D::PrintText(int x, int y, DWORD Format, DWORD Colour, char *Text)
{
	if(!isValid(this->pFont))
	{
		return;
	}

	RECT ptRect;
    SetRect( &ptRect, x, y, x, y );
    pFont->DrawTextA(NULL, Text, -1, &ptRect, Format, Colour);
}

int cD3D::getTextWidth(char *Text)
{
	RECT rc;
	this->pFont->DrawText( NULL, Text, -1, &rc, DT_LEFT | DT_BOTTOM | DT_CALCRECT, D3DCOLOR_XRGB( 255, 255, 255 ) );
	return rc.right - rc.left;
}

void cD3D::DrawLine(int StartX, int StartY, int EndX, int EndY, DWORD Colour)
{
	D3DTLVERTEX vertices[2] = {
		{StartX, StartY, 0, 1.0f, Colour},
		{EndX, EndY, 0, 1.0f, Colour},
	};

	this->pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 2, vertices, sizeof(D3DTLVERTEX));
}

void cD3D::DrawFilledBox(int x, int y, int Width, int Height, DWORD BorderColour, DWORD FillColour)
{
	this->DrawBox(x, y, Width, Height, FillColour);
	this->DrawBorder(x, y, Width, Height, BorderColour);
}

void cD3D::DrawBorder(int x, int y, int Width, int Height, DWORD Colour)
{
	this->DrawLine(x, y, x+Width, y, Colour);
	this->DrawLine(x+Width, y, x+Width, y+Height, Colour);
	this->DrawLine(x, y+Height, x+Width, y+Height, Colour);
	this->DrawLine(x, y, x, y + Height, Colour);
}

void cD3D::DrawBox(int x, int y, int Width, int Height, DWORD Colour)
{
	D3DTLVERTEX vertices[4] = {
		{x		,y		 ,0,1.0f, Colour},
		{x+Width,y		 ,0,1.0f, Colour},
		{x		,y+Height,0,1.0f, Colour},
		{x+Width,y+Height,0,1.0f, Colour}
	};

	DWORD D3D_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	this->pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	this->pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	this->pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	this->pDevice->SetRenderState( D3DRS_FOGENABLE, false );

	this->pDevice->SetFVF(D3D_FVF);
	this->pDevice->SetTexture(0, NULL);

	this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DTLVERTEX));	
}

/*
 * cSprite
 */
void cSprite::drawSprite(int x, int y)
{
	if(isValid(mySprite))
	{
		this->mySprite->Begin(D3DXSPRITE_ALPHABLEND);
		this->mySprite->Draw(myTexture, NULL, NULL, &D3DXVECTOR3(x, y, 0), 0xFFFFFFFF);
		this->mySprite->End();
	}
}

void cSprite::Release()
{
	if(this->mySprite != NULL)
	{
		this->mySprite->Release();
		this->mySprite = NULL;
	}
	if(this->myTexture != NULL)
	{
		this->myTexture->Release();
		this->myTexture = NULL;
	}
}