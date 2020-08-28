#pragma once
#ifndef CGUI_H
#define CGUI_H
#include "stdafx.h"
#include "cD3D.h"

#include <functional>

struct callback_data
{
	void *instance;
	void *callback;
};

class cButton
{
private:
	cD3D *myD3D;

	int posX, posY;
	char *Text;
	int Width, Height;

	POINT CursorPos;

	//Functions related to CallBacks.
	typedef void (*myCallBack)();
	std::vector<callback_data> onClick;
	void addMemberCallBack(void *instance, ...);
public:
	cButton(cD3D *Direct3D, int X, int Y, int W, int H, char *text)
	{
		myD3D = Direct3D;

		posX = X;
		posY = Y;
		Width = W;
		Height = H;
		Text = text;
	}

	void Render(LPDIRECT3DDEVICE9 pDevice);
	void MouseClick(int LMR, bool isDown);
	void KeyPress(short vKey, int Action);

	void setCursorPos(int X, int Y);
	bool MouseInside();

	//Add a Static Callback.
	void addCallBack(myCallBack CallBack)
	{
		callback_data data = { NULL, CallBack };
		onClick.push_back(data);
	};
	//Add a Member Function Call Back;
	template<typename TClass>
	void addCallBack(TClass *instance,	void (__thiscall TClass::*callback)())
	{
		addMemberCallBack(instance, callback);
	};
};

class cTextBox
{
private:
	int PosX, PosY;
	int Width, Height;

	cD3D	*myD3D;
	POINT	MousePos;

	bool showCaret;

	char temp[256];
	std::string Text;
	int TextBoxID;
	int CurrentTextBoxID;

	//Functions related to CallBacks.
	typedef void (*myCallBack)(int ID);
	std::vector<callback_data> onClick;
	void addMemberCallBack(void *instance, ...);
public:
	cTextBox(cD3D *D3D, int ID, int X, int Y, int W, int H)
	{
		myD3D = D3D;
		TextBoxID = ID;

		PosX = X;
		PosY = Y;
		Width = W;
		Height = H;

		Text = "";

		showCaret = false;
	}
	void setText(char *t) { Text = std::string( t ); }

	std::string getString() { return Text; };
	char *getText();

	void Render(LPDIRECT3DDEVICE9 pDevice);

	bool isMouseInside();

	//Call Backs.
	void onKeyPress(LPARAM vKey, bool isDown);
	void onMouseClick(int LMR, bool isDown);
	void setCursorPos(int X, int Y);

	//Add a Static Callback.
	void addCallBack(myCallBack CallBack)
	{
		callback_data data = { NULL, CallBack };
		onClick.push_back(data);
	};
	//Add a Member Function Call Back
	template<typename TClass>
	void addOnClickCallBack(TClass *instance,	void (__thiscall TClass::*callback)(int ID))
	{
		addMemberCallBack(instance, callback);
	}
};
#endif