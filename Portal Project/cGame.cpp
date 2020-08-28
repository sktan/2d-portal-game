#include "stdafx.h"
#include "cGame.h"

//This function will be called every frame and will render the whole game
void cGame::renderFrame(LPDIRECT3DDEVICE9 pDevice)
{
	//Clears the buffer from previous scenes/frames
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 150, 255), 1.0f, 0);
	//Begins the creation of our scene.
	pDevice->BeginScene();

	switch(this->getGameStatus())
	{
	case GAMESTATUS_MainMenu:
		this->myClassManager->getMyMainMenu()->Render(pDevice);
		break;
	case GAMESTATUS_inGame:
		this->myClassManager->getMyPortalGame()->Render(pDevice);
		break;
	case GAMESTATUS_ScoreBoard:
		this->myClassManager->getMyHighScore()->Render(pDevice);
		break;
	case GAMESTATUS_Help:
		this->myClassManager->getMyHelpMenu()->Render(pDevice);
		break;
	default:
		break;
	}
	//Ends the creation of our scene.
	pDevice->EndScene();
	//Presents us with the created scene.
	pDevice->Present(NULL, NULL, NULL, NULL);
}

/*
 * This function changes our 'GameStatus'
 * the GameStatus is basically an ID that tells us which screen
 * we are currently on.
*/
void cGame::changeGameStatus(int GameStatus)
{
	iGameStatus = GameStatus;
	printf("Game Status changed to: %d (cGame::changeGameStatus)\n", GameStatus);
}

//This function gets our GameStatus
int cGame::getGameStatus()
{
	return this->iGameStatus;
}

void cGame::changeMouseLocation(int x, int y)
{
	this->mousePos.x = x;
	this->mousePos.y = y;
}

POINT cGame::getMousePosition()
{
	return this->mousePos;
}

void cGame::onMouseClick(int LMR, bool isMouseDown)
{
	switch(this->getGameStatus())
	{
	case GAMESTATUS_MainMenu:
		this->myClassManager->getMyMainMenu()->onMouseClick(LMR, isMouseDown);
		break;
	case GAMESTATUS_inGame:
		this->myClassManager->getMyPortalGame()->onMouseClick(LMR, isMouseDown);
		break;
	case GAMESTATUS_ScoreBoard:
		this->myClassManager->getMyHighScore()->onMouseClick(LMR, isMouseDown);
		break;
	case GAMESTATUS_Help:
		this->myClassManager->getMyHelpMenu()->onMouseClick(LMR, isMouseDown);
		break;
	}
}

void cGame::onKeyPress(LPARAM vKey, bool isDown)
{
	switch(this->getGameStatus())
	{
	case GAMESTATUS_ScoreBoard:
		this->myClassManager->getMyHighScore()->onKeyPress(vKey, isDown);
		break;
	}
}