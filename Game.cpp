#include "Game.h"

Game::Game() {}

void Game::Initialize(const DisplayWin32& WinDisplay) {
	swapDescriptor.BufferCount = 2;
	swapDescriptor.BufferDesc.Width = WinDisplay.clientWidth;
	swapDescriptor.BufferDesc.Height = WinDisplay.clientHeight;
	swapDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDescriptor.BufferDesc.RefreshRate.Numerator = 60;
	swapDescriptor.BufferDesc.RefreshRate.Denominator = 1;
	swapDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDescriptor.OutputWindow = WinDisplay.hWindow;
	swapDescriptor.Windowed = true;
	swapDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDescriptor.SampleDesc.Count = 1;
	swapDescriptor.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	auto result = D3D11CreateDeviceAndSwapChain(
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
}

void Game::CreateBackBuffer() {

	auto result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);	// __uuidof(ID3D11Texture2D)
	result = device->CreateRenderTargetView(BackBuffer, nullptr, &RenderView);
}

void Game::PrepareResources(std::vector <DirectX::XMFLOAT4> points, std::vector <int> indeces) {

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

}

void Game::Run(const ShadersComponent& shaders) {
	PrevTime = std::chrono::steady_clock::now();

	MSG msg = {};
	bool isExitRequested = false;
	while (!isExitRequested) {

		isExitRequested = MessageHandler(msg);

		Update(shaders);

		RestoreTargets(1, &RenderView);

		float color[] = {totalTime, 0.1f, 0.1f, 1.0f};

		Draw(color);

		RestoreTargets(0);

		swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	}

	std::cout << "Hello World!\n";
}

int Game::MessageHandler(MSG msg) {
	// Handle the windows messages.
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT) {
		return 1;
	}

	return 0;
}

void Game::Update(const ShadersComponent& shaders) {

	context->ClearState();

	context->RSSetState(shaders.rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(swapDescriptor.BufferDesc.Width);
	viewport.Height = static_cast<float>(swapDescriptor.BufferDesc.Height);
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
	context->ClearRenderTargetView(RenderView, color);

	context->DrawIndexed(12, 0, 0);
}

void Game::RestoreTargets(int viewsCnt, ID3D11RenderTargetView* const* RenderView, ID3D11DepthStencilView* DepthStencilView) {
	context->OMSetRenderTargets(viewsCnt, RenderView, DepthStencilView);
}