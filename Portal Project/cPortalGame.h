#pragma once
#include "stdafx.h"

enum MovementDirection
{
	Left = 0,
	Right,
	Down,
	Up
};
class cCharacter
{
private:
	int PositionX, PositionY;
	int Width, Height;
	int Speed;
	float YVelocity;
	bool onGround;

	bool isJumping;

	cSprite		*mySprite;
	cD3D		*myD3D;
public:
	cCharacter(cD3D *D3D, char *SpriteLocation)
	{
		PositionX = 0;
		PositionY = 0;

		myD3D = D3D;
		mySprite = new cSprite(myD3D, SpriteLocation, 30, 68);

		Width = 30;
		Height = 68;

		Speed = 250;

		onGround = false;
		isJumping = false;
		YVelocity = 1.0f;
	}

	int getWidth()	{ return Width; }
	int getHeight()	{ return Height; }
	int getPosX()	{ return PositionX; }
	int getPosY()	{ return PositionY; }

	//Functions related to the Jumping
	void setIsJumping(bool value) { isJumping = value; }
	bool getIsJumping() { return isJumping; }

	//Functions related to the onGround boolean
	void setIsOnGround(bool value) { onGround = value; }
	bool getIsOnGround() { return onGround; }

	//Functions related to YVelocity
	void setYVelocity(float value) { YVelocity = value; }
	float getYVelocity() { return YVelocity; }

	void RelocateCharacter(int X, int Y)
	{
		PositionX = X;
		PositionY = Y;
	}

	void MoveCharacter(int Direction, float Percentage)
	{
		switch(Direction)
		{
		case MovementDirection::Left:
			PositionX -= (int)(Speed * Percentage);
			break;
		case MovementDirection::Right:
			PositionX += (int)(Speed * Percentage);
			break;
		case MovementDirection::Down:
			PositionY += (int)(Speed * Percentage);
			break;
		case MovementDirection::Up:
			PositionY -= (int)(Speed * Percentage);
			break;
		}
	}

	void Render()
	{
		this->mySprite->drawSprite(PositionX, PositionY);
	}
};

class cPortal
{
private:
	int PosX, PosY;
	int Width, Height;
	bool isActive;
	bool bPortalHorizontal;
	DWORD PortalColour;
	int TeleportSide;

	cD3D	*myD3D;
public:
	cPortal (cD3D *D3D, DWORD Colour)
	{
		myD3D = D3D;
		Width = 10;
		Height = 80;

		isActive = false;
		PortalColour = Colour;
	}

	//Portal Arangement
	bool getIsHorizontal() { return bPortalHorizontal; }
	void setIsHorizontal(bool value) { bPortalHorizontal = value; }

	//Set Portal Active
	bool getActive() { return isActive; }
	void setActive(bool value) { isActive = value; }

	//Teleport Location Information
	void setTeleportSide(int value) { TeleportSide = value; setIsHorizontal(value > 1); }
	int getTeleportSide() { return TeleportSide; }

	//Portal Location Information
	void setLocation(int X, int Y){ PosX = X; PosY = Y; }
	int getPosX() { return PosX; }
	int getPosY() { return PosY; }
	int getWidth() { return Width; }
	int getHeight() { return Height; }

	void Render()
	{
		if(isActive)
		{
			if(bPortalHorizontal)
				this->myD3D->DrawBox(PosX, PosY, Height, Width, PortalColour);
			else
				this->myD3D->DrawBox(PosX, PosY, Width, Height, PortalColour);

		}
	}	
};

class cPortalGame
{
	cClassManager	*myClassManager;
	cSprite			*Map;
	cCharacter		*myCharacter;
	SYSTEMTIME		CurrentTime;
	SYSTEMTIME		StartTime;

	std::vector<RECT> ObjectBounds;
	cPortal			*myPortal[2];
	POINT			SpawnLocation;
	POINT			EndLocation;
	std::string		FinishInstruction;

	bool			preQuit;
	bool			bEasyMode;
	SYSTEMTIME		escPressed;
public:
	cPortalGame(cClassManager *ClassManager, bool bEasyMode)
	{
		myClassManager = ClassManager;
		
		preQuit = false;
		LoadLevel("Intro");
		GetSystemTime(&StartTime);

		this->bEasyMode = bEasyMode;
	}

	void LoadLevel(char *LevelName);
	void Render(LPDIRECT3DDEVICE9 pDevice);
	void onMouseClick(int LMR, bool isDown);

	//Game Functions
	bool isInWall(int X, int Y);
	bool isOutOfMap(int X, int Y);
	void handleCollisionDetection(int Direction);
	void handleCharacterMovement();
	void handlePortals();
	void handlePortalTeleportation(int i, int Direction);
	bool checkPlayerOutOfMap();
	bool checkPlayerAtFinish();
	int  getWallSide(int X, int Y);
	void CollisionWithGround();
};

