// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "myMacros.h"

#include "cGUI.h"

#include "cD3D.h"
#include "cFunctions.h"
#include "cMainMenu.h"
#include "cGame.h"
#include "cPortalGame.h"
#include "cHighScore.h"
#include "cHelpMenu.h"

#include "cGameBase.h"

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")
#pragma comment(lib, "dxguid")

//Disable annoying warnings.
#pragma warning(disable: 4244)
#pragma warning(disable: 4482)
#pragma warning(disable: 4996)

#endif 
// TODO: reference additional headers your program requires here
