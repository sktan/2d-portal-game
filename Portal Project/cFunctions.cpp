#include "stdafx.h"
#include "cFunctions.h"

char *cFunctions::getActualFilePath(char *RelativeFilePath)
{
	sprintf(Ret, "%s%s", this->getCurrentDirectory(), RelativeFilePath);
	
	return Ret;
}
char *cFunctions::getCurrentDirectory()
{
	//Get the Executable FilePath.
	//e.g C:\Hello World\myGame.exe
	GetModuleFileName(NULL, CurrentDirectory, sizeof(CurrentDirectory));

	//Find the final '\' in the string.
	//Split the string ending there so we get
	//C:\Hello World
	std::string temp = std::string( CurrentDirectory );
	temp = temp.substr(0, temp.find_last_of("\\"));
	sprintf(CurrentDirectory, "%s", temp.c_str());

	//Return our Directory.
	return CurrentDirectory;
}

float cFunctions::getTimeDifferenceInSeconds(const SYSTEMTIME *Start, const SYSTEMTIME *End)
{
	FILETIME ftStartTime, ftEndTime;
	ULARGE_INTEGER nStartTime, nEndTime;

	SystemTimeToFileTime(Start, &ftStartTime );
	nStartTime.HighPart = ftStartTime.dwHighDateTime;
	nStartTime.LowPart = ftStartTime.dwLowDateTime;
	
	SystemTimeToFileTime (End, &ftEndTime );
	nEndTime.HighPart = ftEndTime.dwHighDateTime;
	nEndTime.LowPart = ftEndTime.dwLowDateTime;

	return ( nEndTime.QuadPart - nStartTime.QuadPart ) / 10000.0f / 1000.0f;
}
