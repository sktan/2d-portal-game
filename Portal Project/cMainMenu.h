#pragma once
#ifndef CMAINMENU_H
#define CMAINMENU_H

#include "stdafx.h"
#include "cGUI.h"

class cClassManager;
class cMainMenu
{
private:
	cClassManager	*myClassManager;
	std::vector<cButton*> myButtons;
	cSprite *GameBanner;

	int iGameStatus;
	POINT MousePos;
public:
	cMainMenu(int GameStatus, cClassManager *ClassManager)
	{
		iGameStatus = GameStatus;
		myClassManager = ClassManager;

		buildGUI();
	}
	void buildGUI();
	void Render(LPDIRECT3DDEVICE9 pDevice);

	void onMouseClick(int LMR, bool isDown);
	int getMyStatusID() { return iGameStatus; };

	//Callbacks
	void StartGame();
	void loadHelp();
	void loadHighScore();
	void loadOptions();
	void exitGame();
};

#endif