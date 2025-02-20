#pragma once

#include <windows.h>
#include <WinUser.h>

class DisplayWin32 {
public:
	DisplayWin32();
	HWND Init(HINSTANCE hInst, LPCWSTR appName);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	
	unsigned int clientHeight;
	unsigned int clientWidth;
	HINSTANCE hInstance;
	HWND hWindow;
	WNDCLASSEX wc;
};