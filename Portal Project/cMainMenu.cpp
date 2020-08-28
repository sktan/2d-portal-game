#include "StdAfx.h"
#include "cMainMenu.h"

/*
 * This function builds our UI.
 * It allows us to add the nessecary UI tools/objects/Controls
 * so we can use it for our game.
*/
void cMainMenu::buildGUI()
{
	cD3D *myD3D = myClassManager->getMyD3D();

	//cButton Params: PositionX, PosittionY, Width, Height, Text
	cButton *tempButton = new cButton(myD3D, 100, 720 - 300 - 80, 426, 150, "START");
	//This Adds a CallBack to the OnClick Event.
	tempButton->addCallBack(this, &cMainMenu::StartGame);
	//Adds the button to our list.
	//This allows us to dynamically handle all our buttons without having to
	//hard code every button individually.
	myButtons.push_back(tempButton);

	//The below code is basically the same thing.

	tempButton = new cButton(myD3D, 1280 - 426 - 100, 720 - 300 - 80, 426, 150, "HELP");
	tempButton->addCallBack(this, &cMainMenu::loadHelp);
	myButtons.push_back(tempButton);

	tempButton = new cButton(myD3D, 100, 720 - 150 - 40, 426, 150, "HIGH SCORE");
	tempButton->addCallBack(this, &cMainMenu::loadHighScore);
	myButtons.push_back(tempButton);

	tempButton = new cButton(myD3D, 1280 - 426 - 100, 720 - 150 - 40, 426, 150, "EXIT GAME");
	tempButton->addCallBack(this, &cMainMenu::exitGame);
	myButtons.push_back(tempButton);

	//Loads the Background Image of our game.
	GameBanner = new cSprite(myD3D, 
		this->myClassManager->getMyFunctions()->getActualFilePath("\\Game Assets\\Main Menu.png"), 1280, 720);
}

/*
 * This Function renders our 'Main Menu'.
 * This will handle all animations/drawing that are shown in this screen.
*/
void cMainMenu::Render(LPDIRECT3DDEVICE9 pDevice)
{
	MousePos = this->myClassManager->getMyGame()->getMousePosition();
	
	GameBanner->drawSprite(0, 0);

	for(std::vector<cButton*>::iterator it = myButtons.begin(); it != myButtons.end(); it ++)
	{
		(*it)->setCursorPos(MousePos.x, MousePos.y);
		(*it)->Render(pDevice);
	}
}

//This funcion handles all the Mouse Clicking.
void cMainMenu::onMouseClick(int LMR, bool isDown)
{
	for(std::vector<cButton*>::iterator it = myButtons.begin(); it != myButtons.end(); it ++)
	{
		(*it)->MouseClick(LMR, isDown);
	}
}

//CallBacks
void cMainMenu::StartGame()
{

	bool bEasyMode = MessageBox(NULL, "Would you like to enable Easy Mode?\n(You will get a 10% penalty to your score)", "Portal Game", MB_YESNO) == IDYES;

	printf("Game Started. (cMainMenu::StartGame(%s))\n", bEasyMode ? "true" : "false");
	if(this->myClassManager->getMyGame() != NULL) ZeroMemory(this->myClassManager->getMyGame(), sizeof(this->myClassManager->getMyGame()));
	cPortalGame *myPortalGame = new cPortalGame(this->myClassManager, bEasyMode);
	this->myClassManager->setPortalGame(myPortalGame);

	this->myClassManager->getMyGame()->changeGameStatus(GAMESTATUS_inGame);
}

void cMainMenu::loadHelp()
{
	this->myClassManager->getMyGame()->changeGameStatus(GAMESTATUS_Help);
}

void cMainMenu::loadHighScore()
{
	cHighScore *myHighScore = new cHighScore(this->myClassManager);
	this->myClassManager->setHighScore(myHighScore);
	
	this->myClassManager->getMyGame()->changeGameStatus(GAMESTATUS_ScoreBoard);
}

void cMainMenu::loadOptions()
{
	DebugMessageBox("Options hasn't been implemented yet");
}

void cMainMenu::exitGame()
{
	if(MessageBox(NULL, "Do you really want to exit the game?", "Portal Game", MB_YESNO) == IDYES)
		ExitProcess(NULL);
}