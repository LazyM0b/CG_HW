#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <chrono>
#include <DirectXMath.h>
#include <vector>
#include <iostream>

#include "DisplayWin32.h"
#include "ShadersComponent.h"
//#include "GameTimer.h"

class Game {
public:

	Game();
	void InitWindow(HINSTANCE hinst, LPCWSTR appName); // done
	void Initialize(); // done
	void PrepareResources(std::vector <DirectX::XMFLOAT4> points, std::vector <int> indeces, ShadersComponent* shaders); //done?
	int MessageHandler(MSG msg); // done
	void Run(const ShadersComponent& shaders);
	void Draw(float* color);
	void PrepareFrame(const ShadersComponent& shaders);
	void Update();
	void UpdateInternal(); //?
	void RestoreTargets(int viewsCnt = 0, ID3D11RenderTargetView* const* RenderView = nullptr, ID3D11DepthStencilView* DepthStencilView = nullptr); // done
	void EndFrame(); //?
	const HWND& MainWindow();
	int Exit();
	void DestroyResources();

	DisplayWin32* display;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	HINSTANCE hInst;
	HWND hWindow;

	DXGI_SWAP_CHAIN_DESC swapDescriptor;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain; 
	ID3D11Texture2D* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderView;
	ID3D11DepthStencilView* depthStencilView; 
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	//GameTimer timer;

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	UINT frameCount = 0;
	UINT clientWidth;
	UINT clientHeight;
};