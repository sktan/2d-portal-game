#include "StdAfx.h"
#include "cHighScore.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <stdio.h>
#include <time.h>


/*
 * This function builds our UI.
 * It allows us to add the nessecary UI tools/objects/Controls
 * so we can use it for our game.
*/
void cHighScore::buildGUI()
{
	myBackgroundImage = new cSprite(this->myClassManager->getMyD3D(), 
		this->myClassManager->getMyFunctions()->getActualFilePath("\\Game Assets\\Leader Board.png"), 1280, 720);
	cButton *tempButton = new cButton(this->myClassManager->getMyD3D(), 950, 139 - (75 / 2), 250, 75, "Submit High Score");

	tempButton->addCallBack(this, &cHighScore::onSubmitClick);
	this->myButtons.push_back(tempButton);

	tempButton = new cButton(this->myClassManager->getMyD3D(), 1280 - 300, 720 - 125, 250, 75, "Back to Main Menu");
	tempButton->addCallBack(this, &cHighScore::onReturnClick);
	this->myButtons.push_back(tempButton);

	this->InputTextBox = new cTextBox(this->myClassManager->getMyD3D(), 1, 385, 139 - 10, 510, 20);
}

/*
 * This function loads all the High Scores from the "High Scores.txt" file
 * The format within the High Score file is (Name Score TimeStamp)
 * The timestamp allows us to store the time as an integer and later converts it back to
 * the tm struct.
*/
void cHighScore::loadHighScore()
{
	std::ifstream file;
	file.open(
		this->myClassManager->getMyFunctions()->getActualFilePath("\\Game Assets\\High Scores.txt"));

	time_t t;
	tm *timestruct;

	HighScoreStruct HSS;
	char Date[45];
	while(file >> HSS.Name >> HSS.Score >> HSS.TimeStamp) 
	{
		//Convert TimeStamp to DD/MM/YY HH:SS AM/PM format
		t = HSS.TimeStamp;
		timestruct = localtime(&t);

		sprintf(Date, "%d/%d/%d %d:%02d %s",
			//DATE
			timestruct->tm_mday, timestruct->tm_mon + 1, (timestruct->tm_year + 1900),
			//TIME
			timestruct->tm_hour < 12 ? timestruct->tm_hour : timestruct->tm_hour - 12, timestruct->tm_min, timestruct->tm_hour < 12 ? "AM" : "PM");
		HSS.Date = std::string(Date);

		//Add the score to our list
		this->HighScores.push_back(HSS);
	}

	//Once we load all the scores, we will sort them from Lowest to Highest.
	this->sortScores();
}

//We will sort the Scores from Lowest to Highest
void cHighScore::sortScores()
{
	//If score is 0 or 1, then we don't have to sort the scores.
	if(HighScores.size() > 1)
	{
		//Temporary Variables.
		//These are important to the sorting of our High Scores
		HighScoreStruct Temp;
		int Size = HighScores.size();
		float TempScore;
		int LowestIndex = 0;

		for(int Index = 0; Index < Size ; Index ++)
		{
			//Set our 'TempScore' to some absurdly high number
			//so we can ALWAYS find the lowest score in the list without
			//running into problems.
			TempScore = (float)INT_MAX;

			for(int Compare = Index; Compare < Size; Compare ++)
			{
				//If our Comparing Score is lower than our Temporary Score,
				//then we will load the set 'TempScore' to the Score we are comparing
				//Then set the 'LowestIndex' to the 'Comparing' index.
				if(HighScores[Compare].Score < TempScore)
				{
					TempScore = HighScores[Compare].Score;
					LowestIndex = Compare;
				}
			}
			
			//If the Lowest Index is equal to our Current Index, then Skip it.
			if(LowestIndex == Index) continue;

			//Swap
			Temp = HighScores[LowestIndex];
			HighScores[LowestIndex] = HighScores[Index];
			HighScores[Index] = Temp;
		}
	}else{
		printf("There were no scores to sort. (cHighScore::sortScores()) \n");
	}
}

/*
 * This function adds a 'Score' to our High Scores list.
 * It will check wether the Username is Empty and the Score is above 1.
 * If it can't add the score, it will return false
 * On the adding of a score, it will return true.
*/
bool cHighScore::addHighScore(char *Username, float Score)
{
	//Check wether the Username is Null or Empty.
	if(Username[0] == '\0' || strcmp(Username, "") == 0)
	{
		return false;
	}

	//Check wether the Score is lower than 1.
	//(Any score lower than 1 shouldn't be possible to achieve.
	if(Score < 1.0f)
	{
		return false;
	}

	HighScoreStruct HSS;
	HSS.Name = Username;
	HSS.Score = Score;
	
	//The below code gets the time in 'DD/MM/YYYY HH:MM AM/PM format.
	char Date[30];
	time_t t = time(NULL);
	HSS.TimeStamp = t;
	tm *timestruct = localtime(&t);

	sprintf(Date, "%d/%d/%d %d:%02d %s", 
		//DATE
		timestruct->tm_mday, timestruct->tm_mon + 1, (timestruct->tm_year + 1900),
		//TIME
		timestruct->tm_hour, (timestruct->tm_min <= 12 ? timestruct->tm_min :  timestruct->tm_min - 12), timestruct->tm_hour < 12 ? "AM" : "PM");
	HSS.Date = Date;
	
	HighScores.push_back(HSS);

	//Create a Character Buffer of 256 characters.
	char buf[256];

	//Iterate through our Scores then save it.
	std::ofstream file;
	file.open(this->myClassManager->getMyFunctions()->getActualFilePath("\\Game Assets\\High Scores.txt"));
	for(std::vector<HighScoreStruct>::iterator it = HighScores.begin(); it != HighScores.end(); it ++)
	{
		sprintf(buf, "%s %.2f %d", it->Name.c_str(), it->Score, it->TimeStamp);
		file << std::string(buf) << std::endl;
	}

	//Free our Memory (256 bytes) after we've used it
	ZeroMemory(buf, sizeof(buf));
	
	//Sort our scores after inserting.
	this->sortScores();

	return true;
}

void cHighScore::Render(LPDIRECT3DDEVICE9 pDevice)
{
	MousePos = this->myClassManager->getMyGame()->getMousePosition();

	//Draw our Background
	this->myBackgroundImage->drawSprite(0, 0);

	//Render our Buttons
	for(std::vector<cButton *>::iterator it = this->myButtons.begin(); it != this->myButtons.end(); it ++)
	{
		(*it)->setCursorPos(MousePos.x, MousePos.y);
		(*it)->Render(pDevice);
	}

	this->myClassManager->getMyD3D()->PrintText(385, 139 - 30, LEFT, 0xFF1010FF, "Enter Your Name: ");
	
	//Print our Score.
	//Will show: Your current score is: 54.32
	char Label[30];
	sprintf(Label, "Your current score is: %.2f", this->CurrentScore);
	this->myClassManager->getMyD3D()->PrintText(385, 139 + 15, LEFT, 0xFF1010FF, Label);
	ZeroMemory(Label, 30);

	//Render our Text Box
	this->InputTextBox->setCursorPos(MousePos.x, MousePos.y);
	this->InputTextBox->Render(pDevice);

	//Display our High Scores
	int i = 0;
	for(std::vector<HighScoreStruct>::iterator HighScore = this->HighScores.begin(); HighScore != this->HighScores.end(); HighScore ++)
	{
		//This will make sure that the 1st and 3rd place are placed vertically in line with eachother.
		if(i == 0 || i == 2)
		{
			sprintf(DisplayHighScore, "%s: %.2f \nDate: %s", HighScore->Name.c_str(), HighScore->Score, HighScore->Date.c_str());
			this->myClassManager->getMyD3D()->PrintText(300, 337 + ((i == 2) * 206), LEFT|DT_VCENTER, 0xFFFF0000, DisplayHighScore);
			i += 1;
		//This will make sure that the 2nd and 4th place are placed vertically in line with eachother.
		}else if(i == 1 || i == 3)
		{
			sprintf(DisplayHighScore, "%s: %.2f \nDate: %s", HighScore->Name.c_str(), HighScore->Score, HighScore->Date.c_str());
			this->myClassManager->getMyD3D()->PrintText(727, 337 + ((i == 3) * 206), LEFT|DT_VCENTER, 0xFFFF0000, DisplayHighScore);
			i += 1;
		}else if(i > 3) break;
		//Exit the loop if the Index is bigger than 3
		//This is so we only show the 4 best scores.
	}
}

//Call Backs start here.
void cHighScore::onKeyPress(LPARAM vKey, bool isDown)
{
	this->InputTextBox->onKeyPress(vKey, isDown);
}

void cHighScore::onMouseClick(int LMR, bool isDown)
{
	this->InputTextBox->onMouseClick(LMR, isDown);
	for(std::vector<cButton *>::iterator it = myButtons.begin(); it != myButtons.end(); it ++)
	{
		(*it)->MouseClick(LMR, isDown);
	}
}

void cHighScore::onSubmitClick()
{
	//check if the textbox has a name in it.
	if(this->InputTextBox->getString().size() < 1)
	{
		DebugMessageBox("Please enter a valid name");
		return;
	}

	//Check if legitimate score.
	if(this->CurrentScore <= 1.0f)
	{
		DebugMessageBox("Your score is invalid.");
		return;
	}

	char Name[30];
	sprintf(Name, "%s", this->InputTextBox->getString().c_str());
	if(this->addHighScore(Name, this->CurrentScore))
	{
		this->InputTextBox->setText("");
		this->SetCurrentScore(0.0f);
		DebugMessageBox("Thank you for inputting your score!");
	}
}

void cHighScore::onReturnClick()
{
	this->myClassManager->getMyGame()->changeGameStatus(::GAMESTATUS_MainMenu);
}