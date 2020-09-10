#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
typedef struct SPoint {
	float x, y;
}TPoint;

TPoint point(float x, float y) {
	TPoint pt;
	pt.x = x;
	pt.y = y;
	return pt;
}
typedef struct Sprite {
	TPoint pos;
	TPoint size;
	COLORREF brush;
	TPoint acc;
} TSprite;

void InitSprite(TSprite *sprite, float xPos, float yPos, float width, float height) {
	sprite->pos = point(xPos, yPos);
	sprite->size = point(width, height);
	sprite->brush = RGB(0, 255, 0);
	sprite->acc = point(0, 0);

}

void DrawSprite(TSprite sprite, HDC dc) {
	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, RGB(0, 0, 0));
	SelectObject(dc, GetStockObject(DC_PEN));
	SetDCPenColor(dc, sprite.brush);
	Rectangle(dc, (int)(sprite.pos.x), (int)(sprite.pos.y), (int)(sprite.pos.x + sprite.size.x) , (int)(sprite.pos.y + sprite.size.y));

}

void MoveSprite(TSprite* sprite) {
	sprite->pos.x += sprite->acc.x;
	sprite->pos.y += sprite->acc.y;
}

TSprite rect;
void Navigation() {
	static int acc = 1;
	rect.acc.x = 0;
	rect.acc.y = 0;
	if (GetKeyState('W') < 0) {
		rect.acc.y = -acc;
	}
	if (GetKeyState('S') < 0) {
		rect.acc.y = acc;
	}
	if (GetKeyState('A') < 0) {
		rect.acc.x = -acc;
	}
		
	if (GetKeyState('D') < 0) {
		rect.acc.x = acc;
	}


}

void WinInit() {
	InitSprite(&rect, 100, 100, 100, 100);
}

void WinMove() {
	Navigation();
	MoveSprite(&rect);

}
void WinShow(HDC dc) {
	
	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, RGB(255, 255, 255));
	Rectangle(dc, 0, 0, 640, 480);
	DrawSprite(rect, dc);
}



LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_DESTROY) {
		PostQuitMessage(0);

	}
	else
	{
		DefWindowProcA(hwnd, message, wparam, lparam);
	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {

	WNDCLASSA wcl;
	memset(&wcl, 0, sizeof(WNDCLASSA));
	wcl.lpszClassName = "my Window";
	wcl.lpfnWndProc = DefWindowProcA;

	RegisterClassA(&wcl);

	HWND hwnd;
	hwnd = CreateWindow(L"my Window", L"Window Name", WS_OVERLAPPEDWINDOW, 10, 10, 640, 480, NULL, NULL, NULL, NULL);
	HDC dc = GetDC(hwnd);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	MSG msg;
	WinInit();
	while (1)
	{
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}
		else {
			WinMove();
			WinShow(dc);
		}


	}
	return(0);

}