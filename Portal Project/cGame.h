#pragma once
#ifndef CGAME_H
#define CGAME_H

#include "stdafx.h"

enum GameStatusEnum{
	GAMESTATUS_MainMenu = 0,
	GAMESTATUS_inGame = 1,
	GAMESTATUS_ScoreBoard = 2,

	GAMESTATUS_Settings = 3,
	GAMESTATUS_settingsAudio = 4,
	GAMESTATUS_settingsHotkeys = 5,

	GAMESTATUS_Help = 10,
} eGameStatus;

class cClassManager;

class cGame
{
private:
	int iGameStatus;
	POINT mousePos;
	SYSTEMTIME StartTime;

	//Different Classes for the games.
	cClassManager	*myClassManager;
public:
	cGame(cClassManager *ClassManager)
	{
		myClassManager = ClassManager;
		iGameStatus = 0;
	}
	
	void renderFrame(LPDIRECT3DDEVICE9 pDevice);

	//Functions related to the mouse.
	void changeMouseLocation(int x, int y);
	POINT getMousePosition();
	void onMouseClick(int LMR, bool mouseDown);

	//Functions related to Keyboard Handling
	void onKeyPress(LPARAM vKey, bool isDown);

	//Functions related to the 'GameStatus'
	int getGameStatus();
	void changeGameStatus(int GameStatus);
};

#endif