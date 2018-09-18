#include <stdio.h>
#include <windows.h>
#include "BackBuffer.h"
#include "ball_library_h.h"
// Store handles to the main window and application
// instance globally.
HDC hdc = 0;
HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;
BackBuffer* gBackBuffer=0;
const int gClientWidth = 800;
const int gClientHeight = 600;
HPEN gGroundPen;
gravity_ball gr_ball;

bool InitMainWindow();
int Run();
void DrawFramesPerSecond(float deltaTime);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Client rectangle dimensions we will use.
// WinMain: Entry point for a Windows application.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR cmdLine, int showCmd)
{
	ghAppInst = hInstance;
	// Create the main window.
	if( !InitMainWindow() ){
		MessageBox(0, "Window Creation Failed.", "Error", MB_OK);
		return 0;
		}
	// Enter the message loop.
	return Run();
}
bool InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "MyWndClassName";
	RegisterClass( &wc );
	// WS_OVERLAPPED | WS_SYSMENU: Window cannot be resized
	// and does not have a min/max button.
	ghMainWnd = ::CreateWindow("MyWndClassName", "MyWindow",WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, 0, 0, ghAppInst, 0);
	if(ghMainWnd == 0)
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return 0;
	}
	ShowWindow(ghMainWnd, SW_NORMAL);
	UpdateWindow(ghMainWnd);
	return true;
}
int Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT)
	{
		// IF there is a Windows message then process it.
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			HDC bbDC = gBackBuffer->getDC();
			// Clear the entire backbuffer black. This gives
			// up a black background.
			SelectObject(bbDC,GetStockObject(BLACK_BRUSH));
			Rectangle(bbDC, 0, 0, 800, 600);
			SelectObject(bbDC, gGroundPen);
			MoveToEx(bbDC, 0,500, 0);
			LineTo(bbDC,800,500);
			SelectObject(bbDC, GetStockObject(GRAY_BRUSH));
			gr_ball.next();
			char ch[20];
			sprintf_s(ch,20,"%f",gr_ball.get_x());
			TextOut(bbDC, 150,0,"X position:",11);
			TextOut(bbDC, 230,0,ch,9);
			sprintf_s(ch,20,"%f",gr_ball.get_y());
			TextOut(bbDC, 150,15,"Y position:",11);
			TextOut(bbDC, 230,15,ch,9);
			TextOut(bbDC, 0,0, "q - increase X",15); 
			TextOut(bbDC, 0,15, "a - decrease X",15); 
			TextOut(bbDC, 0,30, "w - increase Y",15); 
			TextOut(bbDC, 0,45, "s - decrease Y",15);
			Ellipse(bbDC,(int)gr_ball.get_x()-15,(int)gr_ball.get_y()-15,(int)gr_ball.get_x()+15,(int)gr_ball.get_y()+15);
			gBackBuffer->present();
			gr_ball.reset();
			Sleep(10);
		}
	}
	return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LOGPEN lp;
	switch( msg )
	{
		case WM_KEYDOWN:
			switch(wParam)
			{
					case 'Q':
						gr_ball.inc_x();
						break;
					case 'A':
						gr_ball.dec_x();
						break;
					case 'W':
						gr_ball.inc_y();
						break;
					case 'S':
						gr_ball.dec_y();
						break;
			}
			return 0;
		case WM_CREATE:
			lp.lopnColor = RGB(255, 255, 255);
			lp.lopnStyle = PS_SOLID;
			lp.lopnWidth.x = 3;
			lp.lopnWidth.y = 3;
			gGroundPen = CreatePenIndirect(&lp);
			// Create the backbuffer.
			gBackBuffer = new BackBuffer(hWnd,gClientWidth,gClientHeight);
			return 0;
		case WM_DESTROY:
			DeleteObject(gGroundPen);
			delete gBackBuffer;
			PostQuitMessage(0);
			return 0;
	}
	// Forward any other messages we didn't handle to the
	// default window procedure.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
