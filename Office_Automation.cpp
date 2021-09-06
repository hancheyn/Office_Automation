/**************************************************************************
* Title: Automation Tutorial
* Author: Nathan Hanchey
* Date: 08/31/2021
*
* Description: This Program makes it easier to get through Repetitive Processes a little easier
* References: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
* https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
*
***************************************************************************/

#include <iostream>
#include <Windows.h>
#include <string.h>
#include <math.h>

#pragma comment(lib, "user32.lib")

//PART 1
void mousePosition(int* x, int* y);

//PART 2
void keyboard_OpenSketch();
void mouseSwipe(int x0, int y0, int x, int y);
void keyboard_enter();

//PART 3
void keyPress_char(char c);
void mousePress_left(int x, int y);
void keyboard_Save();

//Other
void keyboard_F11();
int relative_position(int X_SIZE, int Y_SIZE, int* x_origin, int* y_origin);
#define X_ORIGIN 1920
#define Y_ORIGIN 1137


//Macros
#define MOUSEEVENT_LEFTDOWN 0x02
#define MOUSEEVENT_LEFTUP 0x04
#define MOUSEEVENT_RIGHTDOWN 0x08
#define MOUSEEVENT_RIGHTUP 0x10

using namespace std;


/*********************************MAIN**************************************/
int main()
{
	int X_SIZE, Y_SIZE;
	char appl[50];

	X_SIZE = GetSystemMetrics(SM_CXFULLSCREEN);
	Y_SIZE = GetSystemMetrics(SM_CYFULLSCREEN);

	//Initialize X and Y
	int X1 = 10;
	int Y1 = 10;
	int X0 = 10;
	int Y0 = 10;

	Sleep(1000);

	//Find Mouse Position
	int pos_x, pos_y;
	char a = 'N';

	/* PART 1
	* Mouse Position
	*/
	printf("\nHover over mouse position, ‘m’ for mouse, and press enter : ");
	mousePosition(&pos_x, &pos_y);
	printf("X = %d & Y = %d\n", pos_x, pos_y);

	Sleep(3000);
	/*
	* PART 2
	* Open Picture Mode of Snip & Sketch
	*/
	keyboard_OpenSketch();
	Sleep(3000);

	//Swipe mouse for picture
	X1 = 1920;
	Y1 = 1137;

	mouseSwipe(X0, Y0, X1, Y1);
	keyboard_enter();
	printf("X = %d & Y = %d\n", X1, Y1);
	Sleep(3000);

	//Copies Picture from Windows Prompt
	system("powershell $(get-clipboard -format image).save(‘save_pic.jpg')");
	Sleep(1000);


	/*PART 3
	* Mouse Position for Start & Open Paint
	*/
	printf("\nHover over START menu, ‘m’ for mouse, and press enter : ");
	mousePosition(&pos_x, &pos_y);
	printf("X = %d & Y = %d\n", pos_x, pos_y);

	mousePress_left(pos_x, pos_y);

	int k = 0;
	strcpy_s(appl, "paint\0");
	for (k = 0; k < 5; k++) {
		keyPress_char(appl[k]);
	}
	Sleep(1000);
	keyboard_enter();
	Sleep(1000);
}


/*********************************FUNCTION DEFINITIONS**************************************/

//PART 1
void mousePosition(int* x, int* y) {

	HWND hwndDesktop = GetDesktopWindow();
	int flag_mouse = 1;
	char _input_;

	while (flag_mouse) {
		//DELAY

		scanf_s("%c", &_input_);
		if ('x' == _input_) {
			return;
		}
		if ('m' == _input_) {
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hwndDesktop, &p);
			*x = p.x;
			*y = p.y;
			cout << "x-Position: " << *x << " | y-position: " << *y << endl;
			flag_mouse = 0;
			Sleep(1000);
		}
	}
}

//PART 2
void mouseSwipe(int x0, int y0, int x, int y) {
	SetCursorPos(x0, y0); //Change to new coordinates
	Sleep(100); //
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(100);
	SetCursorPos(x, y); //Change to new coordinates
	Sleep(100); //
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(100);

}

//Emulates pressing enter on keyboard
void keyboard_enter() {
	//Enter Key Simulation
	INPUT Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = VkKeyScanA(VK_RETURN);
	Input.ki.dwFlags = 0;
	SendInput(1, &Input, sizeof(Input));
	ZeroMemory(&Input, sizeof(Input));
	Input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &Input, sizeof(Input));
	Sleep(100);
}

//Emulates pressing Windows Key shift S on keyboard
void keyboard_OpenSketch() {
	//Enter Key Simulation

	INPUT inputs[6] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_LWIN;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_SHIFT;

	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = VkKeyScanA('s');

	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VkKeyScanA('s');
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[4].type = INPUT_KEYBOARD;
	inputs[4].ki.wVk = VK_SHIFT;
	inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[5].type = INPUT_KEYBOARD;
	inputs[5].ki.wVk = VK_LWIN;
	inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if (uSent != ARRAYSIZE(inputs))
	{
		printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
	}

}


//PART 3
//Emulates pressing F11 on keyboard
void keyboard_F11() {
	//Enter Key Simulation
	INPUT Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = VK_F11;
	SendInput(1, &Input, sizeof(Input));
	ZeroMemory(&Input, sizeof(Input));

	Input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &Input, sizeof(Input));
	Sleep(100);
}

//Inputs x and y coord. for output of single mouse click
void mousePress_left(int x, int y) {
	SetCursorPos(x, y); //Change to new coordinates
	Sleep(100); //
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(100);
}

//Emulates pressing Control S on keyboard
void keyboard_Save() {
	//Enter Key Simulation
	//OutputString(L"Sending 'Win-D'\r\n");
	INPUT inputs[4] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_CONTROL;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VkKeyScanA('s');

	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = VkKeyScanA('s');
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_CONTROL;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if (uSent != ARRAYSIZE(inputs))
	{
		printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
	}
}


//OTHER
//Emulates pressing char key on keyboard
void keyPress_char(char c) {
	INPUT Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = VkKeyScanA(c);
	SendInput(1, &Input, sizeof(Input));
	ZeroMemory(&Input, sizeof(Input));
	Input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &Input, sizeof(Input));
	Sleep(100);
}

int relative_position(int X_SIZE, int Y_SIZE, int* x_origin, int* y_origin) {

	float tempx, tempy;

	tempx = (float)X_SIZE / (float)X_ORIGIN;
	tempy = (float)Y_SIZE / (float)Y_ORIGIN;

	tempx = tempx * (float)*x_origin;
	tempy = tempy * (float)*y_origin;

	*x_origin = tempx;
	*y_origin = tempy;

	return 0;
}
