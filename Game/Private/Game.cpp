#include "Game.h"

Game* Game::instance = nullptr;

Game::Game(HINSTANCE hInst, LPCWSTR appName) : hInstance(hInst), applicationName(appName) {

	clientWidth = 1920;
	clientHeight = 1080;

	display = new DisplayWin32(clientWidth, clientHeight);
	instance = this;
}

void Game::Initialize() {

	input = new InputDevice(this);
	hWindow = display->Init(hInstance, applicationName);

	
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	swapDescriptor.BufferDesc.Width = clientWidth;
	swapDescriptor.BufferDesc.Height = clientHeight;
	swapDescriptor.BufferDesc.RefreshRate.Numerator = 60;
	swapDescriptor.BufferDesc.RefreshRate.Denominator = 1;
	swapDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDescriptor.SampleDesc.Count = 1;
	swapDescriptor.SampleDesc.Quality = 0;
	swapDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDescriptor.BufferCount = 3;
	swapDescriptor.OutputWindow = hWindow;
	swapDescriptor.Windowed = true;
	swapDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDescriptor,
		&swapChain,
		&device,
		nullptr,
		&context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	shaders = new ShadersComponent();

	shaders->Initialize(hWindow, device, context);

	camManager = new CameraManager();
}

void Game::PrepareResources() {
	HRESULT res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	res = device->CreateRenderTargetView(backBuffer, nullptr, &renderView); // second parameter may be not nullptr if it's not for backbuffer

	//create depth/stencil buffer and view

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = clientWidth;
	depthStencilDesc.Height = clientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	res = device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	res = device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
	context->OMSetRenderTargets(1, &renderView, depthStencilView);
}

void Game::Run() {
	//timer.Reset();
	PrevTime = std::chrono::steady_clock::now();
	totalTime = 0;
	frameCount = 0;

	MSG msg = {};
	bool isExitRequested = false;

	while (!isExitRequested) {

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (MessageHandler(msg.message)) {
			isExitRequested = 1;
			continue;
		}

		//timer.Tick();

		auto	curTime = std::chrono::steady_clock::now();
		float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
		PrevTime = curTime;

		Update(deltaTime);

		context->ClearState();

		PrepareFrame();

		RestoreTargets(1, &renderView, depthStencilView);

		Draw();

		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);

		RestoreTargets();
	}
}

int Game::MessageHandler(UINT msg) {
	switch (msg) {
	case WM_QUIT:
		return 1;
	}
	return 0;
}

void Game::PrepareFrame() {

	D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT();
	viewport->Width = (float) clientWidth;
	viewport->Height = (float) clientHeight;
	viewport->TopLeftX = 0;
	viewport->TopLeftY = 0;
	viewport->MinDepth = 0;
	viewport->MaxDepth = 1.0f; 
	context->RSSetViewports(1, viewport);
}

void Game::Update(float deltaTime) {


	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;

		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(swapDescriptor.OutputWindow, text);

		frameCount = 0;
	}

}

void Game::Draw() {

	float color[] = { 0.1, 0.1f, 0.2f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	shaders->Draw(context);
}

void Game::RestoreTargets(int viewsCnt, ID3D11RenderTargetView* const* RenderView, ID3D11DepthStencilView* DepthStencilView) {
	context->OMSetRenderTargets(viewsCnt, RenderView, DepthStencilView);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 1);
}

void Game::DestroyResources() {

}

int Game::Exit() {

	DestroyResources();

	return 1;
}

const HWND& Game::MainWindow() {
	return hWindow;
}