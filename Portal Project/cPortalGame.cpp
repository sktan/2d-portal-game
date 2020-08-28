#include "StdAfx.h"
#include "cPortalGame.h"
#include <iostream>
#include <fstream>
#include <string>

/*
 * This function handles the loading of our levels.
 * It will load the map, the colisions, our 'Spawn Point'
 * and any other information that will be nessecary for the game to run.
*/
void cPortalGame::LoadLevel(char *LevelName)
{
	myPortal[0] = new cPortal(myClassManager->getMyD3D(), 0xFFE77F1E);
	myPortal[1] = new cPortal(myClassManager->getMyD3D(), 0xFF0284FF);

	char RelativeLocation[100];
	sprintf(RelativeLocation, "%s\\Game Assets\\Maps\\%s\\Level.png", 
		this->myClassManager->getMyFunctions()->getCurrentDirectory(), LevelName);

	this->Map = new cSprite(this->myClassManager->getMyD3D(), RelativeLocation, 1280, 720);

	this->myCharacter = new cCharacter(this->myClassManager->getMyD3D(),
		this->myClassManager->getMyFunctions()->getActualFilePath("\\Game Assets\\Sprite.png"));

	//Load Map Walls File
	std::ifstream myFile;
	sprintf(RelativeLocation, "%s\\Game Assets\\Maps\\%s\\Walls.txt", 
		this->myClassManager->getMyFunctions()->getCurrentDirectory(), LevelName);
	myFile.open(RelativeLocation);

	//Iterate through each line of the file,
	//then get the object bounds through there.
	RECT r;
	if(ObjectBounds.size() != 0) ObjectBounds.clear();
	while(myFile >> r.left >> r.top >> r.right >> r.bottom)
	{
		ObjectBounds.push_back(r);
	}

	std::string Begin;
	std::string End;

	sprintf(RelativeLocation, "%s\\Game Assets\\Maps\\%s\\LocPoints.txt", 
		this->myClassManager->getMyFunctions()->getCurrentDirectory(), LevelName);
	std::ifstream LocationFile;
	LocationFile.open(RelativeLocation);

	while(LocationFile >> Begin >> r.left >> r.top >> r.right >> r.bottom >> End)
	{
		if(strcmp(Begin.c_str(), "SPAWN") == 0) 
		{	
			this->SpawnLocation.x = r.left;
			this->SpawnLocation.y = r.top;
		}else if(strcmp(Begin.c_str(), "END") == 0)
		{
			this->EndLocation.x = r.left;
			this->EndLocation.y = r.top;

			FinishInstruction = End;
		}
	}
	
	this->myCharacter->RelocateCharacter(this->SpawnLocation.x, this->SpawnLocation.y);
}

float getAngleBetweenPoints(POINT Target, POINT Start)
{
	D3DXVECTOR3 Delta;
	Delta.x = Target.x - Start.x;
	Delta.z = Target.y - Start.y;

	if(Delta.z == 0 && Delta.x == 0) return 0;

	float Angle;
	Angle = (atan2(Delta.z, -Delta.x) * 180.0f / MATH_PI) - 90.0f;

	if(Angle > 180.0f) Angle -= 360.0f;
	else if( Angle < -180.0f) Angle += 360.0f;

	return Angle;
}

/*
 * This function will handle the rendering of our game.
 * Sprite Animation, Displaying of the map/portals/character and
 * any other effects relevant to the game will be displayed here
*/
void cPortalGame::Render(LPDIRECT3DDEVICE9 pDevice)
{
	this->Map->drawSprite(0, 0);

	this->handleCharacterMovement();

	if(this->bEasyMode)
	{
		POINT myLoc = {this->myCharacter->getPosX() + (this->myCharacter->getWidth() / 2), this->myCharacter->getPosY() + (this->myCharacter->getHeight() / 2)};
		float fAngle = DegToRad(getAngleBetweenPoints(myClassManager->getMyGame()->getMousePosition(), myLoc));

		float NewX = myLoc.x + (sinf(fAngle));
		float NewY = myLoc.y + (cosf(fAngle));

		while(!this->isInWall(NewX, NewY))
		{
			NewX += (sinf(fAngle));
			NewY += (cosf(fAngle));

			//Don't bother creating a portal if the portal goes out of the map.
			if(this->isOutOfMap(NewX, NewY)) break;
		}

		this->myClassManager->getMyD3D()->DrawLine(myLoc.x, myLoc.y, NewX, NewY, 0xFF00FF00);
	}
	this->myCharacter->Render();

	for(int i = 0; i < 2; i ++) this->myPortal[i]->Render();

	GetSystemTime(&CurrentTime);

	if(!this->preQuit && (GetAsyncKeyState(VK_ESCAPE) &1))
	{
		this->preQuit = true;
		GetSystemTime(&escPressed);
	}
	if(this->preQuit)
	{
		float QuitTimer = this->myClassManager->getMyFunctions()->getTimeDifferenceInSeconds(&escPressed, &CurrentTime);
		if(QuitTimer < 2.50f)
		{
			this->myClassManager->getMyD3D()->PrintText((1280 / 2), 20, CENTER|DT_BOTTOM, 0xFFFF0000, "Press \"Escape\" again to quit.");

			//Create a cool slider that allows us to see how long we have
			//till we decide wether we quit or not.
			int Width = this->myClassManager->getMyD3D()->getTextWidth("Press \"Escape\" again to quit.");
			Width = Width * (QuitTimer / 2.50f);
			this->myClassManager->getMyD3D()->DrawBox((1280 / 2) - (Width / 2), 20, Width, 3, 0xFFFFFF00);

			//If we press Escape during the 2.5 seconds,
			//We will switch our 'GameStatus' back to the Main Menu.
			if(GetAsyncKeyState(VK_ESCAPE) &1)
			{
				this->myClassManager->getMyGame()->changeGameStatus(GAMESTATUS_MainMenu);
			}
		}else{
			this->preQuit = false;
		}
	}
}

/*
 * This function handles the collision check for our game.
 * It will detect wether the character within our game is in a valid position and not in walls.
*/
void cPortalGame::handleCollisionDetection(int Direction)
{
	//Get the bounds of myCharacter.
	RECT myRect;
	myRect.left = this->myCharacter->getPosX();
	myRect.top = this->myCharacter->getPosY();
	myRect.right = myRect.left + this->myCharacter->getWidth();
	myRect.bottom = myRect.top + this->myCharacter->getHeight();

	//Iterate through all our Object Bounds
	for(std::vector<RECT>::iterator it = ObjectBounds.begin(); it != ObjectBounds.end(); it ++)
	{		
		switch(Direction)
		{
		case MovementDirection::Left:
			//Character's Left hand side has colided with the objects right hand side.
			if(myRect.left < (*it).right && myRect.left > (*it).left && myRect.bottom > (*it).top && myRect.top < (*it).bottom)
			{
				this->myCharacter->RelocateCharacter((*it).right, this->myCharacter->getPosY());
			}
			break;
		case MovementDirection::Right:
			//Character's Right hand side has colided with the objects Left hand Side
			if(myRect.right > (*it).left && myRect.right < (*it).right
				&& myRect.bottom > (*it).top && myRect.top < (*it).bottom)
			{
				this->myCharacter->RelocateCharacter((*it).left - this->myCharacter->getWidth(), this->myCharacter->getPosY());
			}
			break;
		case MovementDirection::Down:
			//Character's bottom side has colided with the objects top side.
			if(myRect.bottom > (*it).top && myRect.bottom < (*it).bottom
				&& (myRect.right > (*it).left && myRect.right < (*it).right
				|| myRect.left < (*it).right && myRect.left > (*it).left))
			{
				this->myCharacter->RelocateCharacter(myRect.left, (*it).top - this->myCharacter->getHeight());
				this->CollisionWithGround();
			}
			break;
		case MovementDirection::Up:
			//Character's Top side has collided with the objects bottom side.
			if(myRect.top < (*it).bottom && myRect.top > (*it).top
				&& (myRect.right > (*it).left && myRect.right < (*it).right
				|| myRect.left < (*it).right && myRect.left > (*it).left))
			{
				this->myCharacter->RelocateCharacter(myRect.left, (*it).bottom);
			}
			break;
		}
	}
}

/*
 * This function handles what happens when you go into a portal.
 * The Direction parameter tells this function which way the character is headed
 * so we can determine where the character will need to teleport to.
 * e.g. Person moves right into the left hand side of the First Portal
 *		Person will teleport to the Right hand side of the Second Portal
*/
void cPortalGame::handlePortals()
{
	//Get the bounds of myCharacter.
	RECT myRect;
	myRect.left = this->myCharacter->getPosX();
	myRect.top = this->myCharacter->getPosY();
	myRect.right = myRect.left + this->myCharacter->getWidth();
	myRect.bottom = myRect.top + this->myCharacter->getHeight();

	RECT portalRect;
	for(int i = 0; i < 2; i ++)
	{
		if(!this->myPortal[i]->getActive() || !this->myPortal[!i]->getActive()) return;

		if(!this->myPortal[i]->getIsHorizontal())
		{
			//Handle Vertical Portal
			portalRect.left = this->myPortal[i]->getPosX();
			portalRect.top = this->myPortal[i]->getPosY();
			portalRect.right = this->myPortal[i]->getWidth() + portalRect.left;
			portalRect.bottom = portalRect.top + this->myPortal[i]->getHeight();
		}else{
			//Then Horizontal Portals
			portalRect.left = this->myPortal[i]->getPosX();
			portalRect.top = this->myPortal[i]->getPosY();
			portalRect.right = portalRect.left + this->myPortal[i]->getHeight(); 
			portalRect.bottom = portalRect.top + this->myPortal[i]->getWidth();
		}

		//If our character is touching a portal
		if(((myRect.left < portalRect.right && myRect.left > portalRect.left) || (myRect.right > portalRect.left && myRect.right < portalRect.right))
			&& myRect.bottom > portalRect.top && myRect.top < portalRect.bottom)
		{
			switch(this->myPortal[!i]->getTeleportSide())
			{
			case MovementDirection::Left:
				this->myCharacter->RelocateCharacter(this->myPortal[!i]->getPosX() - this->myCharacter->getWidth() - 2,
					this->myPortal[!i]->getPosY() + 5);
				break;
			case MovementDirection::Right:
				this->myCharacter->RelocateCharacter(this->myPortal[!i]->getPosX() + 12,
					this->myPortal[!i]->getPosY() + 5);
				break;
			case MovementDirection::Down:
				this->myCharacter->RelocateCharacter(this->myPortal[!i]->getPosX() + (this->myPortal[!i]->getWidth() / 2),
					this->myPortal[!i]->getPosY() + 12);
				break;
			case MovementDirection::Up:
				this->myCharacter->RelocateCharacter(this->myPortal[!i]->getPosX() + (this->myPortal[!i]->getWidth() / 2),
					this->myPortal[!i]->getPosY() - this->myCharacter->getHeight() - 12);
				break;
			}
		}
	}
}

/*
 * This function determines which side of a wall the X and Y coordinates are closest to.
 * It will only return a number greater than -1 (not valid) if the coordinates given are
 * within an object.
*/
int cPortalGame::getWallSide(int x, int y)
{
	int ret = -1;

	int differences[4];
	int TempDistance = 1280;

	for(std::vector<RECT>::iterator it = ObjectBounds.begin(); it != ObjectBounds.end(); it ++)
	{
		if(x > (*it).left && x < (*it).right && y > (*it).top && y < (*it).bottom)
		{
			differences[MovementDirection::Left] = x - (*it).left;
			differences[MovementDirection::Right] = (*it).right - x;

			differences[MovementDirection::Up] = y - (*it).top;
			differences[MovementDirection::Down] = (*it).bottom - y;
			for(int i = 0; i < 4; i ++)
			{
				if(differences[i] < TempDistance) 
				{
					TempDistance = differences[i];
					ret = i;
				}
			}
			break;
		}
	}

	return ret;
}

/*
 * This function checks wether a coordinate is within a wall.
*/
bool cPortalGame::isInWall(int x, int y)
{
	for(std::vector<RECT>::iterator it = ObjectBounds.begin(); it != ObjectBounds.end(); it ++)
	{
		if(x > (*it).left && x < (*it).right && y < (*it).bottom && y > (*it).top) return true;
	}

	return false;
}

/* 
 * This function checks wether a coordinate is outside of the map.
*/
bool cPortalGame::isOutOfMap(int x, int y)
{
	return (x > 1280 || x < 0 || y > 720 || y < 0);
}
/*
 * This function handles all of the player movement.
 * It will determine how far the character needs to move, and will also handle
 * the jumping and falling of the characters.
*/
void cPortalGame::handleCharacterMovement()
{
	//Press TAB to get rid of all portals.
	if(GetAsyncKeyState(VK_TAB) &1)
	{
		for(int i = 0; i < 2; i ++) myPortal[i]->setActive(false);
	}

	SYSTEMTIME time;
	GetSystemTime(&time);

	float percentage = this->myClassManager->getMyFunctions()->getTimeDifferenceInSeconds(&CurrentTime, &time);

	//If there was more than a .5 second delay between a frame.
	//we will skip this function
	if(percentage > 0.5f) return;

	//Move Left
	if(GetAsyncKeyState('A') <0)
	{
		this->myCharacter->MoveCharacter(MovementDirection::Left, percentage);

		this->handlePortals();
		this->handleCollisionDetection(MovementDirection::Left);
	}

	//Move Right
	if(GetAsyncKeyState('D') <0)
	{
		this->myCharacter->MoveCharacter(MovementDirection::Right, percentage);

		this->handlePortals();
		this->handleCollisionDetection(MovementDirection::Right);
	}

	//Jump
	if(GetAsyncKeyState(VK_SPACE) &1 && !this->myCharacter->getIsJumping() && this->myCharacter->getIsOnGround())
	{
		this->myCharacter->setIsJumping(true);
		this->myCharacter->setYVelocity(0.0f);
	}

	//This will check whether the character is jumping
	if(this->myCharacter->getIsJumping())
	{
		this->myCharacter->MoveCharacter(MovementDirection::Up, percentage);
		
		this->handleCollisionDetection(MovementDirection::Up);

		if(this->myCharacter->getYVelocity() < 15.0f)
			this->myCharacter->setYVelocity((percentage * 4) + this->myCharacter->getYVelocity());
	}

	if(!this->myCharacter->getIsJumping())
	{
		this->myCharacter->setIsOnGround(this->isInWall(this->myCharacter->getPosX(), 
			this->myCharacter->getPosY() + this->myCharacter->getHeight() + 3));

		if(!this->myCharacter->getIsOnGround())
		{
			if(this->myCharacter->getYVelocity() < 15.0f)
				this->myCharacter->setYVelocity((percentage * 4) + this->myCharacter->getYVelocity());
		}
	}

	//Gravity
	if(percentage <= 1.0f)
	{
		this->myCharacter->MoveCharacter(MovementDirection::Down, percentage * this->myCharacter->getYVelocity());

		this->handlePortals();
		this->handleCollisionDetection(MovementDirection::Down);
	}

	//Check wether the player is out of the map.
	//If so, we'll relocate him back to the spawn
	if(this->checkPlayerOutOfMap()) 
	{
		this->myCharacter->RelocateCharacter(this->SpawnLocation.x, this->SpawnLocation.y);
		this->myPortal[0]->setActive(false);
		this->myPortal[1]->setActive(false);

		printf("Player has fallen out of map %.02f seconds into the game \n", 
			this->myClassManager->getMyFunctions()->getTimeDifferenceInSeconds(&this->StartTime, &this->CurrentTime));
	}
	

	if(this->checkPlayerAtFinish()) 
	{
		if(strcmp(this->FinishInstruction.c_str(), "FINISH") == 0)
		{
			SYSTEMTIME Now;
			GetSystemTime(&Now);

			float Score = this->myClassManager->getMyFunctions()->getTimeDifferenceInSeconds(&StartTime, &Now);
			if(this->bEasyMode) Score *= 1.10f;

			this->myClassManager->getMyGame()->changeGameStatus(GAMESTATUS_ScoreBoard);
			this->myClassManager->setHighScore(new cHighScore(this->myClassManager));
			this->myClassManager->getMyHighScore()->SetCurrentScore(Score);
		}else{
			char LevelName[25];
			sprintf(LevelName, "%s", this->FinishInstruction.c_str());
			this->LoadLevel(LevelName);
			ZeroMemory(LevelName, sizeof(LevelName));
		}
	}
}

/*
 * This function gets called every time the player hits the ground.
*/
void cPortalGame::CollisionWithGround()
{
	if(this->myCharacter->getIsJumping() || !this->myCharacter->getIsOnGround())
	{
		this->myCharacter->setIsJumping(false);
		this->myCharacter->setIsOnGround(true);

		this->myCharacter->setYVelocity(1.0f);
	}
}

bool cPortalGame::checkPlayerOutOfMap()
{
	return (this->myCharacter->getPosX() < 0 || this->myCharacter->getPosX() > 1280
		|| this->myCharacter->getPosY() < 0 || this->myCharacter->getPosY() > 720);
}

bool cPortalGame::checkPlayerAtFinish()
{
	RECT myRect;
	myRect.left = this->myCharacter->getPosX();
	myRect.top = this->myCharacter->getPosY();
	myRect.right = myRect.left + this->myCharacter->getWidth();
	myRect.bottom = myRect.top + this->myCharacter->getHeight();

	return (myRect.right > this->EndLocation.x && myRect.left < this->EndLocation.x
		&& myRect.bottom > this->EndLocation.y && myRect.top < this->EndLocation.y);
}

/*
 * LMR: 0  = Left Click
 * LMR: 1  = Middle Click
 * LMR: 2  = Right Click
 *
 * This function will be called every time the game is clicked on.
 * It will handle the shooting and placement of both the portals
*/
void cPortalGame::onMouseClick(int LMR, bool isDown)
{
	POINT p = this->myClassManager->getMyGame()->getMousePosition();

	//Check wether the mouse wasn't middle clicked.
	//Also, because the event gets called twice during a click (one on mouse down, one on mouse up)
	//the isDown check makes sure that we don't do the same action twice.
	if(LMR != 1 && isDown)
	{
		//This will find the middle of the characters sprite.
		POINT myLoc = {this->myCharacter->getPosX() + (this->myCharacter->getWidth() / 2), this->myCharacter->getPosY() + (this->myCharacter->getHeight() / 2)};
		
		float Yaw = DegToRad(getAngleBetweenPoints(p, myLoc));

		float NewX = myLoc.x + (sinf(Yaw));
		float NewY = myLoc.y + (cosf(Yaw));

		while(!this->isInWall(NewX, NewY))
		{
			NewX += (sinf(Yaw));
			NewY += (cosf(Yaw));

			//Don't bother creating a portal if the portal goes out of the map.
			if(this->isOutOfMap(NewX, NewY)) return;

			if(this->isInWall(NewX, NewY))
			{
				//(LMR == 2) returns us 0 on Left Click, and 1 on Right Click.
				//This also allows us to get Portal[0] and Portal[1] without having
				//to write the code twice.

				//This area of code determines which side of the wall the portal was shot on.
				//Then places the portal accordingly
				//It also makes sure that the portals are not within another wall and places it in a 'valid' area.
				this->myPortal[(LMR == 2)]->setTeleportSide(this->getWallSide(NewX, NewY));
				switch(this->myPortal[(LMR == 2)]->getTeleportSide())
				{
				case MovementDirection::Left:
					for(NewY; NewY != 0; NewY --) 
					{
						if(!this->isInWall(NewX - 3, NewY + this->myPortal[LMR == 2]->getHeight())) 
						{
							if(!this->isOutOfMap(NewX - 3, NewY + this->myPortal[LMR == 2]->getHeight()))
							{
								break;
							}
						}
					}
					this->myPortal[(LMR == 2)]->setLocation(NewX - 2, NewY);
					break;
				case MovementDirection::Right:
					for(NewY; NewY != 0; NewY --) 
					{
						if(!this->isInWall(NewX + 10, NewY + this->myPortal[LMR == 2]->getHeight())) 
						{
							if(!this->isOutOfMap(NewX + 10, NewY + this->myPortal[LMR == 2]->getHeight()))
							{
								break;
							}
						}
					}
					this->myPortal[(LMR == 2)]->setLocation(NewX - 8, NewY);
					break;
				case MovementDirection::Up:
					for(NewX; NewX != 0; NewX --) 
					{
						if(!this->isInWall(NewX + this->myPortal[LMR == 2]->getHeight(), NewY - 5)) 
						{
							if(!this->isOutOfMap(NewX + this->myPortal[LMR == 2]->getHeight(), NewY - 5)) 
							{
								break;
							}
						}
					}
					this->myPortal[(LMR == 2)]->setLocation(NewX, NewY - 2);
					break;
				case MovementDirection::Down:
					for(NewX; NewX != 0; NewX --)	
					{
						if(!this->isOutOfMap(NewX + this->myPortal[LMR == 2]->getHeight(), NewY + 10)) 
						{
							break;
						}
					}
					this->myPortal[(LMR == 2)]->setLocation(NewX, NewY - 8);
					break;
				}

				//Activate the portal
				this->myPortal[(LMR == 2)]->setActive(true);
				break;
			}
		}
	}

	//Check wether the mouse was middle clicked.
	if(LMR == 1 && !isDown)
	{
		printf("Coordinates of Middle Click: X(%d) Y(%d) (cPortalGame::OnMouseClick)\n", p.x, p.y);
	}
}