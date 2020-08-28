#pragma once

#define isValid(x) (x != NULL)

#define LMouseFix  if(GetAsyncKeyState(VK_LBUTTON)){GetAsyncKeyState(VK_RBUTTON);}
#define RMouseFix  if(GetAsyncKeyState(VK_RBUTTON)){GetAsyncKeyState(VK_LBUTTON);}

#define lParamToHorizontal(x)	(( int )( short )LOWORD(x))
#define lParamToVertical(x)		(( int )( short )HIWORD(x))

#define DebugMessageBox(x) MessageBox(NULL, x, "Portal Project", NULL);

#define MATH_PI 3.141592654f
#define DegToRad( degree ) ( (degree) * (MATH_PI / 180.0f) )

#define ValidRect(x) (x).right > 0 && (x).bottom > 0