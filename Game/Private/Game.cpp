#include "Game.h"

Game* Game::instance = nullptr;

Game::Game(HINSTANCE hInst, LPCWSTR appName): hInstance(hInst), applicationName(appName) {
	display = new DisplayWin32();
	instance = this;
}

void Game::Initialize(UINT objCnt) {

	input = new InputDevice(this);
	hWindow = display->Init(hInstance, applicationName);

	clientWidth = 640;
	clientHeight = 480;
	
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferDesc.Width = clientWidth;
	swapDesc.BufferDesc.Height = clientHeight;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 3;
	swapDesc.OutputWindow = hWindow;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr,
		&context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	if (!meshes.empty()) {
		for (int i = 0; i < objCnt; ++i) {
			std::vector<Vector4> colors;

			Vector3 color((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
			colors.push_back(Vector4(color.x, color.y, color.z, 1.0f));
			colors.push_back(Vector4(color.z, color.y, color.x, 1.0f));

			objects.push_back(new GameComponent());
			if (!(this->applicationName == L"SolarSys"))
				objects[i]->Initialize(device, meshes[i]);
			else
				objects[i]->Initialize(device, meshes[i], colors, LOD);
		}
	}
	else exit(1);

	shaders = new ShadersComponent();

	shaders->Initialize(hWindow, device, context);

	camManager = new CameraManager(2000.0f);
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

	float color[] = { 0.5, 0.5f, 0.5f, 1.0f };
	context->ClearRenderTargetView(renderView, color);
	shaders->Draw(context);

	D3D11_MAPPED_SUBRESOURCE res = {};
	Matrix data;

	for (int i = 0; i < objects.size(); ++i) {

		context->VSSetConstantBuffers(0, 1, &objects[i]->worldPosBuffer);

		context->Map(objects[i]->worldPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

		auto dataPtr = reinterpret_cast<float*>(res.pData);
		memcpy(dataPtr, &data, sizeof(data));

		objects[i]->Draw(context);

		context->Unmap(objects[i]->worldPosBuffer, 0);
	}

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