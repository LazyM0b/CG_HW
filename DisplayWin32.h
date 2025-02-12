#pragma once

#include <windows.h>
#include <WinUser.h>

class DisplayWin32 {
public:
	DisplayWin32();
	DisplayWin32(LPCWSTR appName, HINSTANCE hInst, int windowHeight, int windowWidth);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	
	unsigned int clientHeight;
	unsigned int clientWidth;
	HINSTANCE hInstance;
	HWND hWindow;
	WNDCLASSEX wc;
};