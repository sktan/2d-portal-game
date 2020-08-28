#pragma once
#include "stdafx.h"

class cFunctions
{
private:
	char Ret[MAX_PATH];
	char CurrentDirectory[MAX_PATH];
public:
	cFunctions()
	{
	}
	char *getActualFilePath(char *RelativeFileLocation);
	char *getCurrentDirectory();
	float getTimeDifferenceInSeconds(const SYSTEMTIME *Start, const SYSTEMTIME *End);
};
