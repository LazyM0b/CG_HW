#include "Game.h"

Game::Game() {}

void Game::Initialize() {
	LPCWSTR appName = L"My3DApp";
	hWindow = display->Init(hInst, appName);

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

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
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

void Game::InitWindow(HINSTANCE hInst, LPCWSTR appName) {
	hInst = hInst;
	display = new DisplayWin32();
}

void Game::PrepareResources(std::vector <DirectX::XMFLOAT4> points, std::vector <int> indeces, ShadersComponent* shaders) {

	//create vertex shader

	

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	//create input layout

	device->CreateInputLayout(
		inputElements,
		2,
		shaders->vertexShaderByteCode->GetBufferPointer(),
		shaders->vertexShaderByteCode->GetBufferSize(),
		&shaders->layout);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	HRESULT res = device->CreateRasterizerState(&rastDesc, &shaders->rastState);
	context->RSSetState(shaders->rastState);

	//vertex buffer initialization
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = &points.front();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	//index buffer initialization
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = &indeces.front();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	device->CreateVertexShader(
		shaders->vertexShaderByteCode->GetBufferPointer(),
		shaders->vertexShaderByteCode->GetBufferSize(),
		nullptr, &shaders->vertexShader);

	//create index shader

	device->CreatePixelShader(
		shaders->pixelShaderByteCode->GetBufferPointer(),
		shaders->pixelShaderByteCode->GetBufferSize(),
		nullptr, &shaders->pixelShader);
}

void Game::Run(const ShadersComponent& shaders) {
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
		if (msg.message == WM_QUIT) {
			isExitRequested = 1;
			continue;
		}

		//timer.Tick();

		PrepareFrame(shaders);

		Update();

		RestoreTargets(1, &renderView);

		float color[] = {totalTime, 0.1f, 0.1f, 1.0f};

		Draw(color);

		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);

		RestoreTargets();
	}

	std::cout << "Hello World!\n";
}

int Game::MessageHandler(MSG msg) {
	// Handle the windows messages.
	return 0;
}

void Game::PrepareFrame(const ShadersComponent& shaders) {

	context->ClearState();

	context->RSSetState(shaders.rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = (float) clientWidth;
	viewport.Height = (float) clientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	context->IASetInputLayout(shaders.layout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &shaders.strides, &shaders.offsets);
	context->VSSetShader(shaders.vertexShader, nullptr, 0);
	context->PSSetShader(shaders.pixelShader, nullptr, 0);
}

void Game::Update() {

	auto	curTime = std::chrono::steady_clock::now();
	float	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

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

void Game::Draw(float* color) {
	context->ClearRenderTargetView(renderView, color);

	context->DrawIndexed(16, 0, 0);
}

void Game::RestoreTargets(int viewsCnt, ID3D11RenderTargetView* const* RenderView, ID3D11DepthStencilView* DepthStencilView) {
	context->OMSetRenderTargets(viewsCnt, RenderView, DepthStencilView);
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