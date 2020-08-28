#include "StdAfx.h"
#include "cHelpMenu.h"

void cHelpMenu::buildGUI()
{
	this->myBackground = new cSprite(this->myClassManager->getMyD3D(),
		this->myClassManager->getMyFunctions()->getActualFilePath("\\Game Assets\\Help Screen.png"), 1280, 720);

	this->myBackButton = new cButton(this->myClassManager->getMyD3D(), 1280 - 300, 720 - 100, 250, 75, "Back to Main Menu");
	this->myBackButton->addCallBack(this, &cHelpMenu::onBackClick);
}

void cHelpMenu::Render(LPDIRECT3DDEVICE9 pDevice)
{
	this->myBackground->drawSprite(0, 0);
	this->myBackButton->Render(pDevice);
	this->myBackButton->setCursorPos(this->myClassManager->getMyGame()->getMousePosition().x, this->myClassManager->getMyGame()->getMousePosition().y);
}

void cHelpMenu::onMouseClick(int LMR, bool isDown)
{
	this->myBackButton->MouseClick(LMR, isDown);
}

void cHelpMenu::onBackClick()
{
	this->myClassManager->getMyGame()->changeGameStatus(GAMESTATUS_MainMenu);
}