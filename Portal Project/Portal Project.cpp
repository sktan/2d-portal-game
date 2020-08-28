// Portal Project.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

//Definitions for the Classes that will be used for the Portal Project.
cFunctions		*myFunctions = NULL;
cGame			*myGame = NULL;
cD3D			*myD3D = NULL;
cMainMenu		*myMainMenu = NULL;
cClassManager	*myClassManager = NULL;
cHelpMenu		*myHelpMenu;

//Redefinitions for our functions.
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEMTIME StartTime, EndTime;
	GetSystemTime(&StartTime);

	SetConsoleTitle("Steven Tan's Portal Project");
	printf("Steven Tan's Portal Project is now initializing. \n");
	printf("\n");

	printf("Arguments: \n");
	for(int i = 0; i < argc; i ++)
		printf("Program started with arg[%d]: %s \n", i, argv[i]);

	printf("\n");

	//Boolean check whether the initialisation was a success.
	bool initSuccess = true;

	//Here we will attempt to initialise myClassManager.
	printf("Initialising cClassManager *myClassManager... \n");
	myClassManager = new cClassManager();
	if(isValid(myClassManager))
	{
		printf("myClassManager initialised at: 0x%0.8X \n", myClassManager);
	}else{
		initSuccess = false;
		printf("myClassManager failed to initialise. \n");
	}


	printf("Creating Window... \n");

	//Initialise the Window
	WNDCLASS wc = {0};
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = MainWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = _T("form");
	ATOM a = RegisterClass(&wc);

	//This will indicate the Width and Height for our Window
	RECT r = {0, 0, 1280, 720};

	//The Window Style would be a Normal Form without the Maximize button and the functionability to resize the form
	DWORD style = (WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME)|WS_VISIBLE;
	AdjustWindowRect(&r, style, FALSE);
	HWND hwnd = NULL;
	hwnd = CreateWindow(MAKEINTATOM(a), _T("Portal Project"), style, 0, 0, r.right - r.left, r.bottom - r.top, NULL, NULL, wc.hInstance, NULL);

	if(isValid(hwnd))
	{
		printf("Window creation success. \n");
		myClassManager->setHWND(hwnd);
	}else{
		printf("Window failed to create. \n");
		initSuccess = false;
	}

	//Here we will attempt to initialise myD3D.
	printf("Initialising cD3D *myD3D... \n");
	myD3D = new cD3D();
	if(isValid(myD3D))
	{
		printf("myD3D initialised at: 0x%0.8X \n", myD3D);
		myClassManager->setD3D(myD3D);
	}else{
		printf("myD3D failed to initialise. \n");
		initSuccess = false;
	}

	printf("Initialising the Direct3D Interface \n");
	myD3D->createDevice(hwnd);

	if(isValid(myD3D->getDirectDevice()))
	{
		printf("Direct3D Device initialised at: 0x%0.8X \n", myD3D->getDirectDevice());
		//We will need to make sure our DirectDevice is valid before we create our font.
		myD3D->CreateFont(16, "Verdana");
	}else{
		printf("Direct3D Device failed to initialise. \n");
		initSuccess = false;
	}

	//Here we will attempt to initialise myFunctions.
	printf("Initialising cFunctions *myFunctions... \n");
	myFunctions = new cFunctions();
	if(isValid(myFunctions)) 
	{
		printf("myFunctions initialised at: 0x%0.8X \n", myFunctions);
		myClassManager->setFunctions(myFunctions);
	}else{
		initSuccess = false;
		printf("myFunctions failed to initialise. \n");
	}

	//Here we will attempt to initiate myMainMenu.
	printf("Initialising cMainMenu *myMainMenu... \n");
	myMainMenu = new cMainMenu(GAMESTATUS_MainMenu, myClassManager);
	if(isValid(myMainMenu))
	{
		printf("myMainMenu initialised at: 0x%0.8X \n", myMainMenu);
		myClassManager->setMainMenu(myMainMenu);
	}else{
		printf("myMainMenu failed to initialise. \n");
		initSuccess = false;
	}

	//Here we will attempt to initialise myGame.
	printf("Initialising cGame *myGame... \n");
	myGame = new cGame(myClassManager);
	if(isValid(myGame))
	{
		printf("myGame initialised at: 0x%0.8X \n", myGame);
		myClassManager->setGameClass(myGame);
	}else{
		initSuccess = false;
		printf("myGame failed to initialise. \n");
	}

	printf("Initialising cHelpMenu *myHelpMenu... \n");
	myHelpMenu = new cHelpMenu(myClassManager);
	if(isValid(myHelpMenu))
	{
		printf("myHelpMenu initialised at: 0x%0.8X \n", myHelpMenu);
		myClassManager->setHelpMenu(myHelpMenu);
	}else{
		initSuccess = false;
		printf("myHelpMenu failed to initialise. \n");
	}

	//End Initialisation.
	GetSystemTime(&EndTime);
	if(initSuccess)
	{
		printf("\n");
		printf("Initialisation of the Portal Project was successful. \n");
		printf("Initialisation took %.3f second(s). \n", myFunctions->getTimeDifferenceInSeconds(&StartTime, &EndTime));
	}else{
		printf("Initialisation of the Portal Project was unsuccessful. \n");
	}

	RECT WindowRect;
	GetWindowRect(hwnd, &WindowRect);

	WindowRect.top += (WindowRect.top - r.top);
	WindowRect.bottom -= 5;
	WindowRect.left += 5;
	WindowRect.right -= 5;
	ClipCursor(&WindowRect);
	printf("\n");

	//Print myDirectory.
	printf("Detected Installation Directory: %s \n", myFunctions->getCurrentDirectory());

	//Main application loop.
	MSG msg;
	while(msg.message != WM_QUIT)
	{	
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		
		myGame->renderFrame(myD3D->getDirectDevice());
	}
	return 0;
}

void handleMouseClickEvents(UINT msg)
{
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		myGame->onMouseClick(0, true);
		break;
	case WM_LBUTTONUP:
		myGame->onMouseClick(0, false);
		break;

	case WM_MBUTTONDOWN:
		myGame->onMouseClick(1, true);
		break;
	case WM_MBUTTONUP:
		myGame->onMouseClick(1, false);
		break;

	case WM_RBUTTONDOWN:
		myGame->onMouseClick(2, true);
		break;
	case WM_RBUTTONUP:
		myGame->onMouseClick(2, false);
		break;
	}
}

//The function that gets called on ANY Window Messages.
//Includes Close, move and paint.
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_ACTIVATE:
			if(wParam == TRUE)
			{
				RECT WindowRect;
				GetWindowRect(hwnd, &WindowRect);

				WindowRect.top += 25;
				WindowRect.bottom -= 5;
				WindowRect.left += 5;
				WindowRect.right -= 5;
				ClipCursor(&WindowRect);
			}else if(wParam == FALSE){
				ClipCursor(NULL);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN: case WM_MBUTTONUP:
			handleMouseClickEvents(msg);
			break;
		case WM_MOUSEMOVE:
			//Convert the lParam to X and Y coordinates.
			myGame->changeMouseLocation(lParamToHorizontal(lParam), lParamToVertical(lParam));
			break;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			myGame->onKeyPress(LPARAM(wParam), true);
			break;
		case WM_KEYUP: case WM_SYSKEYUP:
			myGame->onKeyPress(LPARAM(wParam), false);
			break;
		case WM_PAINT:
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}