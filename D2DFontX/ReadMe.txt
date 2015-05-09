/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D2DFontX D2D1 Font Library Wrapper
Author: Paul Skertich
Release Date: May 9, 2015
Version 1.0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Description:

D2D1FontX is a simple D2D1 Font Wrapper Library. Easy to use. 

Examples:
//--Global Declaration.
D2DFontX *fontX;

FontSettings structure:
{
name of the font family: IMPACT.
size of the font: 16.0f;
Font color FONTCOLOR(1,1,1,1); = WHITE.
Font Weight;
Font Style;

};
bool InitFont(IDXGISwapChain *swapchain, FontSettings &fontSettings, HWND windowHandle) {
//--windowHandle is passed to grab the client's height and width.
fontX = new D2DFontX(swapchain, fontSettings, windowHandle);
if(fontX) 
	return true;
else
    return false;
} 
//-- Inside render loop.
bool renderFont(std::wstring text) {
      fontX->RenderText(text);
	  
}

//-- When program is quitting.
void DisposeFontX() {
	//-- Clean up after yourself.
     if(fontX) fontX->Dispose();
	 if(fontX) delete fontX;
	 
}