#pragma once
#include "stdafx.h"
#include <vector>

struct HighScoreStruct
{
	std::string Name;
	float Score;
	std::string Date;
	int TimeStamp;
};

class cHighScore
{
	cClassManager	*myClassManager;
	std::vector<HighScoreStruct> HighScores;
	std::vector<cButton *> myButtons;
	cTextBox		*InputTextBox;
	cSprite			*myBackgroundImage;

	float CurrentScore;

	char DisplayHighScore[256];
	POINT MousePos;
public:
	cHighScore(cClassManager *ClassManager)
	{
		myClassManager = ClassManager;

		CurrentScore = 0.0f;
		loadHighScore();
		buildGUI();
	}

	//Renders the UI of our High Score Screen.
	void Render(LPDIRECT3DDEVICE9 pDevice);

	//This function builds the UI of our High Score.
	void buildGUI();

	//Loads the Score list
	void loadHighScore();
	//This function will be sorting the scores from Lowest to highest.
	//Why lowest to highest?
	//We will be sorting the scores from lowest to highest because our best scores will
	//be the people who took the shortest time to complete the game.
	void sortScores();
	//This function will insert the score to the score-list.
	//It will also return a value of 'true' when the function inserts data
	//into our score list.
	bool addHighScore(char *Username, float Score);

	//This function will set our Current Score.
	//This allows us to pass the score information from our Game to our High Score board.
	void SetCurrentScore(float Score) { CurrentScore = Score; }

	//Callbacks
	void onKeyPress(LPARAM vKey, bool isDown);
	void onMouseClick(int LMR, bool isDown);
	void onSubmitClick();
	void onReturnClick();
};
