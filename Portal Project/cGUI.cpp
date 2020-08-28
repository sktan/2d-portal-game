#include "stdafx.h"
#include "cGUI.h"
#include <time.h>
#include <string>

/*
 * cButton
*/
void cButton::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if(!MouseInside())
	{
		myD3D->DrawBox(posX, posY, Width, Height, D3DCOLOR_ARGB(150, 0, 0, 0));
		myD3D->PrintText(posX + (Width / 2), posY + (Height / 2), CENTER|DT_VCENTER, D3DCOLOR_XRGB(255, 0, 0), this->Text);
	}else{
		myD3D->DrawBox(posX - 20, posY - 20, Width + 40, Height + 40, D3DCOLOR_ARGB(200, 0, 0, 0));
		myD3D->PrintText(posX + (Width / 2), posY + (Height / 2), CENTER|DT_VCENTER, D3DCOLOR_XRGB(255, 0, 0), this->Text);
	}
}

void cButton::MouseClick(int LMR, bool isDown)
{
	if(this->MouseInside())
	{
		for(std::vector<callback_data>::iterator it = onClick.begin(); it != onClick.end(); it ++)
		{
			if(LMR == 0 && !isDown)
			{
				if(it->instance)
				{
					void *inst = it->instance;
					void *func = it->callback;
					__asm
					{
						//Moves our Class Instance to the counter
						//This allows us to use local members within our class.
						mov ecx, inst;
						//Moves our Function Pointer to the accumulator.
						//This allows us to later call our function
						mov eax, func;
						//Calls our function
						call eax;
					}
				}else{
					((myCallBack)it->callback)();
				}
			}
		}
	}
}

bool cButton::MouseInside()
{
	return (CursorPos.x > posX && CursorPos.x < posX + Width && CursorPos.y > posY && CursorPos.y < posY + Height);
}

void cButton::setCursorPos(int X, int Y)
{
	CursorPos.x = X;
	CursorPos.y = Y;
}

void cButton::addMemberCallBack(void *instance, ...)
{
	if(instance)
	{
		va_list args;
		va_start(args, instance);

		callback_data data = { instance, va_arg(args, void*) };
		this->onClick.push_back(data);

		va_end(args);
	}
}

/*
 * cTextBox
*/

void cTextBox::Render(LPDIRECT3DDEVICE9 pDevice)
{
	//Convert String to Char Array so we can use the 'PrintText' function
	//To draw the text.
	if(this->showCaret && this->CurrentTextBoxID == this->TextBoxID) sprintf(temp, "%s|", this->Text.c_str());
	else sprintf(temp, "%s", this->Text.c_str());

	//Change the Caret Shown/Hidden every half a second.
	this->showCaret = ((GetTickCount() % 1000) > 500);

	this->myD3D->DrawFilledBox(PosX, PosY, Width, Height, 0xFF0000FF, 0xBB000000);
	this->myD3D->PrintText(PosX + 5, PosY + (Height / 2), LEFT|DT_VCENTER, D3DCOLOR_XRGB(255, 0, 0), this->temp);
}

void cTextBox::setCursorPos(int X, int Y)
{
	MousePos.x = X;
	MousePos.y = Y;
}

bool cTextBox::isMouseInside()
{
	return (MousePos.x > PosX && MousePos.x < PosX + Width
		&& MousePos.y > PosY && MousePos.y < PosY + Height);
}

void cTextBox::onMouseClick(int LMR, bool isDown)
{
	if(this->isMouseInside())
	{
		this->CurrentTextBoxID = this->TextBoxID;
		for(std::vector<callback_data>::iterator it = onClick.begin(); it != onClick.end(); it ++)
		{
			if(LMR == 0 && !isDown)
			{
				if(it->instance)
				{
					void *inst = it->instance;
					void *func = it->callback;
					__asm
					{
						//Moves our Class Instance to the counter
						//This allows us to use local members within our class.
						mov ecx, inst;
						//Moves our Function Pointer to the accumulator.
						//This allows us to later call our function
						mov eax, func;
						//Pushes our TextBoxID variable to the stack.
						push TextBoxID;
						//Calls our function
						//FunctionName(TextBoxID)
						call eax;
					}
				}else{
					((myCallBack)it->callback)(this->TextBoxID);
				}
			}
		}
	}else{
		this->CurrentTextBoxID = 0;
	}
}

/*
 * This is the function that gets called every time the game
 * receives a keystroke.
*/
void cTextBox::onKeyPress(LPARAM vKey, bool isDown)
{
	if(this->CurrentTextBoxID == this->TextBoxID)
	{
		if(isDown)
		{
			//Valid KeyStrokes for our TextBox.
			//This includes A - Z and 0 - 9
			if(((vKey <= 'Z' && vKey >= 'A') || (vKey >= '0' && vKey <= '9')) && this->Text.size() < 30) 
			{
				if((GetAsyncKeyState(VK_SHIFT) < 0 || (GetKeyState(VK_CAPITAL) & 0x0001) !=0))
				{
					this->Text.push_back(char(vKey));
				}else{
					this->Text.push_back(char(tolower(vKey)));
				}
			}

			if(vKey == VK_BACK && this->Text.size() > 0)
			{
				this->Text.erase(this->Text.size() - 1, 1);
			}
		}
	}
}

void cTextBox::addMemberCallBack(void *instance, ...)
{
	if(instance)
	{
		va_list args;
		va_start(args, instance);

		callback_data data = { instance, va_arg(args, void*) };
		this->onClick.push_back(data);

		va_end(args);
	}
}