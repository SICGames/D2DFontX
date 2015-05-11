#include "D2D1FontDemo.h"


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#define SAFE_RELEASE(x) if(x) x->Release();

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowExW(NULL,
		wc.lpszClassName,
		L"Basic D2D1FontX Demo",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	if (!initScene(hWnd)) {
		MessageBox(0, L"Unable to intiailize D3D Device. Quitting now!", L"D3D Device Failed.", 0);
		releaseScene();
		DestroyWindow(hWnd);
	}


	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		renderScene();
	}

	// clean up DirectX and COM
	releaseScene();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool initD2D1FontX(IDXGISwapChain *g_Swapchain, FontSettings &fontSettings, HWND window) {
	
	d2dFontX = new D2DFontX(g_Swapchain, fontSettings, window);
	if (!d2dFontX) 
		return false;

	return true;
}
void renderText(std::wstring text) {
	if (d2dFontX){
		d2dFontX->RenderText(text);

	}
}
// this function initializes and prepares Direct3D for use
bool initScene(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	HRESULT hr = S_OK;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.SampleDesc.Quality = 0;                             // multisample quality level
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	IDXGIFactory1 *pFactory;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1*), (void**)&pFactory);
	if (FAILED(hr))
		return false;

	IDXGIAdapter *pAdapter;
	hr = pFactory->EnumAdapters(0, &pAdapter);
	if (FAILED(hr))
		return false;

	DXGI_ADAPTER_DESC pAdapterDesc;
	hr = pAdapter->GetDesc(&pAdapterDesc);
	if (FAILED(hr))
		return false;

	SAFE_RELEASE(pFactory);

	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(pAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	if (FAILED(hr)) {
		return false;
	}
	
	SAFE_RELEASE(pAdapter);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	hr = dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	if (FAILED(hr)) {
		return false;
	}
	SAFE_RELEASE(pBackBuffer);

	
	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
	FontSettings fontSettings = FontSettings(L"Impact", 16.0f, FONTCOLOR(1, 1, 1, 1), DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);

	if (!initD2D1FontX(swapchain, fontSettings, hWnd)) {
		MessageBox(0, L"Failed to initialize D2D1Font.", L"D2D1FontX Failed", 0);
		return false;
	}
	d2dFontX->setWorldTransform(D2D1::Matrix3x2F::Identity());

	return true;

}


// this is the function used to render a single frame
void renderScene(void)
{
	float bgColor[4] = { 0, 0.2f, 0.4f, 0.0f };
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, bgColor);
	
	renderText(L"Well Hello there Stranger! How are you today?");

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void releaseScene()
{
	if (d2dFontX) {
		d2dFontX->Dipose();
		delete d2dFontX;
	}
	SAFE_RELEASE(swapchain);
	SAFE_RELEASE(devcon);
	SAFE_RELEASE(dev);

}