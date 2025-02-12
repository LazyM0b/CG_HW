#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <chrono>
#include <DirectXMath.h>
#include <vector>
#include <iostream>

#include "DisplayWin32.h"
#include "ShadersComponent.h"

class Game {
public:

	Game();
	void Initialize(const DisplayWin32& WinDisplay);
	void CreateBackBuffer();
	void PrepareResources(std::vector <DirectX::XMFLOAT4> points, std::vector <int> indeces);
	void PrepareFrame();
	int MessageHandler(MSG msg);
	void Run(const ShadersComponent& shaders);
	void Draw(float* color);
	void Update(const ShadersComponent& shaders);
	void UpdateInternal();
	void RestoreTargets(int viewsCnt, ID3D11RenderTargetView* const* RenderView = nullptr, ID3D11DepthStencilView* DepthStencilView = nullptr);
	void EndFrame();
	void Exit();
	void DestroyResources();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	ID3D11DeviceContext* context;
	DXGI_SWAP_CHAIN_DESC swapDescriptor;
	IDXGISwapChain* swapChain; 
	ID3D11Texture2D* BackBuffer;
	ID3D11RenderTargetView* RenderView;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float totalTime = 0;
	unsigned int frameCount = 0;
};