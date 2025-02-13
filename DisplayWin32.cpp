#include "DisplayWin32.h"
#include "iostream"

LRESULT CALLBACK DisplayWin32::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

DisplayWin32::DisplayWin32() {}

void DisplayWin32::Initialize(LPCWSTR appName, HINSTANCE hInst, int windowHeight, int windowWidth) {

	//initialize window class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	hInstance = hInst;
	clientHeight = windowHeight;
	clientWidth = windowWidth;

	// Register the window class.
	RegisterClassEx(&wc);

	// Initialize window rectangle
	RECT windowRect = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - clientHeight) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - clientWidth) / 2;

	hWindow = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);

	// Show window on screen
	ShowWindow(hWindow, SW_SHOW);
	SetForegroundWindow(hWindow);
	SetFocus(hWindow);

	ShowCursor(true);
		
}