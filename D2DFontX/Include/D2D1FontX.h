#ifndef __D2D1_FONT_X___
#define __D2D1_FONT_X___

//-- Inludes Must Point to Windows SDK not DirectX SDK.

#include <dwrite.h>
#include <dxgi.h>
#include <d2d1.h>
#include <vector>
#include <string>
#include <sstream>

#if _WIN32
	#include <Windows.h> //-- Must be included or atlbase and atlcom will bitch about undefined macros.
	#include <atlbase.h>
	#include <atlcom.h>
#endif

#ifndef FONTX_DLL_EXPORT
#define FONTX_API __declspec(dllexport)
#else 
#define FONTX_API __declspec(dllimport);
#endif

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")

#if defined(UNICODE)
	typedef const wchar_t WIDESTRING;
	typedef WIDESTRING* LPWIDESTRING;
#endif
#define SAFE_DISPOSE(x) if(x) { x->Release(); }

	class FONTX_API FONTCOLOR {
	private:
		float _red, _green, _blue, _alpha;
		float _color[4];
	public:
		FONTCOLOR() : _red(0), _green(0), _blue(0), _alpha(0) {}
		FONTCOLOR(float red, float green, float blue, float alpha) 
		{
			this->_red = red; this->_green = green; this->_blue = blue; this->_alpha = alpha;
		}

		FONTCOLOR(FONTCOLOR &fontColor) {
			this->_red = fontColor.Red();
			this->_green = fontColor.Green();
			this->_blue = fontColor.Blue();
			this->_alpha = fontColor.Alpha();
		}

		float Red() { return _red; }
		float Green() { return _green; }
		float Blue() { return _blue; }
		float Alpha() { return _alpha; }
		const float * getColor() { return _color; }

	};

//-- Must include the dwrite.lib, dxgi.lib and d2d1.lib
struct FONTX_API FontSettings {
	FontSettings() : fontFamily(L""), fontSize(0), fontColor(FONTCOLOR(0,0,0,0)), fontWeight(), fontStyle() {};

	FontSettings(std::wstring font_Family, float font_size, FONTCOLOR font_color, DWRITE_FONT_WEIGHT font_weight, DWRITE_FONT_STYLE font_style) : fontFamily(font_Family), 
		fontSize(font_size), fontColor(font_color), fontWeight(font_weight), fontStyle(font_style) {
	}

	std::wstring fontFamily;
	DWRITE_FONT_WEIGHT fontWeight;
	DWRITE_FONT_STYLE fontStyle;

	float fontSize;
	FONTCOLOR fontColor; //-- Red, Green, Blue, Alpha.
};

class FONTX_API D2DFontX {
private:
	ID2D1SolidColorBrush *TextBrush;
	ID2D1RenderTarget *TextRenderTarget;
	IDWriteFactory *DwriteFactory;
	ID2D1Factory *D2D1Factory;
	IDWriteTextFormat *TextFormat;
	FONTCOLOR fontColor;
	D2D1::Matrix3x2F World, translation, rotation, scale, skew;

public:

	// @-- Just a simple constructor.
	D2DFontX();
	// @-- Usage: D2DFontX fontX = new D2DFontX(myD3D11Swapchain, myFontSettings);
	// @-- Usage #2: D2DFontX fontX = new D2DFontX(myD3D11SwapChain,FontSettings(L"Impact",16.0f,FONTCOLOR(1.0f,1.0f,1.0f,1.0f));
	D2DFontX(IDXGISwapChain *backbuffer11, FontSettings &settings, HWND windowHandle);
	// @-- Usage: D2DFontX fontX = new D2DFontX(); 
	// @-- fontX->Create(myD3D11Swapchain, myfontSettings);
	bool Create(IDXGISwapChain *swapchain, FontSettings &settings, HWND windowHandle);
	//-- @-- Usage: RenderText(L"Hello World!"); [INSIDE RENDER LOOP OF APPLICATION OR GAME ENGINE.
	//-- LPWIDESTRING = const wchar_t* 
	//-- WIDESTRING = const wchar_t
	void RenderText(std::wstring text);
	//--Set the translate point to X Y AXIS.
	void Translate(float x, float y);
	//-- Set the scale point X Y in Center Point.
	void Scale(float x, float y, float centerX, float centerY);
	//-- Set the rotation.
	void Rotate(float x, float y, float angle);
	//-- Set the Skew
	void Skew(float x, float y, float centerX, float centerY);
	//-- get the rotation matrix.
	D2D1::Matrix3x2F getRotationMatrix() const { return rotation; }
	//-- get the translation matrix
	D2D1::Matrix3x2F getTranslationMatrix() const { return translation; }
	//-- get the scale matrix
	D2D1::Matrix3x2F getScaleMatrix() const { return scale;}
	//-- get the skew matrix
	D2D1::Matrix3x2F getSkewMatrix() const { return skew; }
	//-- set the World Transform how ever you like. (E.G) !!!! --- THE MATRIXS MUST BE SET BEFORE OBTAINING WORLD TRANSFORMATION !!!!
	//-- D2D1::Matrix3x2F world = fontX->getRotationMatrix() * fontX->getTranslationMatrix();
	//-- setWorldTransform(world);
	void setWorldTransform(D2D1::Matrix3x2F &worldtransform);
	
	//-- Disposes FontX Objects.
	void Dipose();
	float clientHeight, clientWidth;
	
};

#endif
