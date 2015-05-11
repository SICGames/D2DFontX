#ifndef __D2D1__FONT_DEMO__
// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <d3d11.h>
#include <D3DX11.h>
#include "../../Include/D2D1FontX.h"
#pragma comment(lib, "../../Lib/Debug/D2D1FontX.lib")


// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer

D2DFontX *d2dFontX;

bool initD2DFontX(IDXGISwapChain *g_Swapchain, FontSettings &fontsettings, HWND windowHandle);
void renderText(std::wstring text);
void renderScene(void);
bool initScene(HWND window);

void releaseScene();

#endif
