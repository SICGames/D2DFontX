#include "D2D1FontX.h"

D2DFontX::D2DFontX() : D2D1Factory(0), DwriteFactory(0), TextFormat(0), TextBrush(0), TextRenderTarget(0) {

}
D2DFontX::D2DFontX(IDXGISwapChain *swapchain, FontSettings &settings, HWND windowHandle) {
	this->Create(swapchain, settings, windowHandle);
}
bool D2DFontX::Create(IDXGISwapChain *d3d11SwapChain, FontSettings &settings, HWND windowHandle) {
	
	HRESULT result = S_OK;
	fontColor = settings.fontColor;

	RECT windowDimension;
	GetClientRect(windowHandle,&windowDimension);
	clientHeight = ((float)windowDimension.bottom - windowDimension.top);
	clientWidth = ((float)windowDimension.right - windowDimension.left);

	// And lets create our D2D factory and DWrite factory at this point as well, that way if any of them fail we'll fail out completely.
	auto options = D2D1_FACTORY_OPTIONS();
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &D2D1Factory);
	if (FAILED(result)) {
		return false;
	}

	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown * *>(&DwriteFactory));
	if (FAILED(result)) {
		return false;
	}

	if (d3d11SwapChain == nullptr)
		return false;

	CComPtr<IDXGISurface1> d2dRT;
	result = d3d11SwapChain->GetBuffer(0, IID_PPV_ARGS(&d2dRT));
	if (FAILED(result)) {
		return false;
	}
	
	auto d2dRTProps = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 0, 0);
	result = D2D1Factory->CreateDxgiSurfaceRenderTarget(d2dRT, &d2dRTProps, &TextRenderTarget);
	if (FAILED(result)) {

		return false;
	}


	result = DwriteFactory->CreateTextFormat(settings.fontFamily.c_str(), nullptr, settings.fontWeight, settings.fontStyle, DWRITE_FONT_STRETCH_NORMAL, settings.fontSize, L"", &TextFormat);
	if (FAILED(result)) {
		return result;
	}

	result = TextRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1,1,0,1), &TextBrush);
	if (FAILED(result)) {
		return result;
	}

	return true;

}
void D2DFontX::RenderText(std::wstring text) {

	TextRenderTarget->BeginDraw();

	//Create our string
	std::wostringstream printString;
	printString << text;
	Text = printString.str();

	//Set the Font Color
	D2D1_COLOR_F FontColor = D2D1::ColorF(fontColor.Red(),fontColor.Green(),fontColor.Blue(),fontColor.Alpha());
	//Set the brush color D2D will use to draw with
	TextBrush->SetColor(FontColor);
	//Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, clientWidth,clientHeight);

	//Draw the Text
	TextRenderTarget->DrawTextW(
		Text.c_str(),
		wcslen(Text.c_str()),
		TextFormat,
		layoutRect,
		TextBrush
		);

	TextRenderTarget->EndDraw();
	
	return;

}
void D2DFontX::Dipose() {
	SAFE_DISPOSE(TextBrush);
	SAFE_DISPOSE(TextFormat);
	SAFE_DISPOSE(TextRenderTarget);
	SAFE_DISPOSE(DwriteFactory);
	SAFE_DISPOSE(D2D1Factory);

}