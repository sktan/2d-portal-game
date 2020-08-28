#pragma once
#ifndef CGAMEBASE_H
#define CGAMEBASE_H

#include "stdafx.h"

class cClassManager
{
private:
	cD3D			*myD3D;
	cFunctions		*myFunctions;
	cGame			*myGame;
	cMainMenu		*myMainMenu;
	cPortalGame		*myPortalGame;
	cHighScore		*myHighScore;
	cHelpMenu		*myHelpMenu;
	HWND			windowHwnd;
public:
	cClassManager()
	{
	}

	void setHWND(HWND hwnd)
	{
		windowHwnd = hwnd;
	}
	HWND getHwnd()
	{
		return windowHwnd;
	}

	//Function to set the D3D
	void setD3D(cD3D *D3D)
	{
		myD3D = D3D;
	}
	cD3D *getMyD3D()
	{
		return myD3D;
	}

	void setFunctions(cFunctions *Functions)
	{
		myFunctions = Functions;
	}
	cFunctions *getMyFunctions()
	{
		return myFunctions;
	}

	void setGameClass(cGame *Game)
	{
		myGame = Game;
	}
	cGame *getMyGame()
	{
		return myGame;
	}

	void setMainMenu(cMainMenu *MainMenu)
	{
		myMainMenu = MainMenu;
	}
	cMainMenu *getMyMainMenu()
	{
		return myMainMenu;
	}

	void setPortalGame(cPortalGame *PortalGame)
	{
		myPortalGame = PortalGame;
	}
	cPortalGame *getMyPortalGame()
	{
		return myPortalGame;
	}

	void setHighScore(cHighScore *HighScore)
	{
		myHighScore = HighScore;
	}
	cHighScore *getMyHighScore()
	{
		return myHighScore;
	}

	void setHelpMenu(cHelpMenu *HelpMenu)
	{
		myHelpMenu = HelpMenu;
	}
	cHelpMenu *getMyHelpMenu()
	{
		return myHelpMenu;
	}

};

//This will be the 'Base-Class' for the rendering of our game.
//We will use this class as a template for the rest of our classes
class cGameBase
{
private:
	cClassManager	*myClassManager;
public:
	cGameBase(cClassManager	*ClassManager)
	{
		myClassManager = ClassManager;
	}

	void Render(LPDIRECT3DDEVICE9 pDevice);
	void MouseClick(int x, int y, bool isDown);
	void KeyPress(short vKey, int Action);
};

#endif