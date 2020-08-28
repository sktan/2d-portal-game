#pragma once
#include "stdafx.h"

class cHelpMenu
{
	cClassManager	*myClassManager;
	cButton			*myBackButton;
	cSprite			*myBackground;
public:
	cHelpMenu(cClassManager *ClassManager)
	{
		myClassManager = ClassManager;
		buildGUI();
	}

	void Render(LPDIRECT3DDEVICE9 pDevice);
	void buildGUI();

	void onMouseClick(int LMR, bool isDown);

	void onBackClick();
};
