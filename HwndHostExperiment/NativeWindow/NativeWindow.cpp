// NativeWindow.cpp : 定义控制台应用程序的入口点。
//

#include <Windows.h>

#include "D3DCore.h"
#include <exception>
#include <cstdio>

#pragma comment(lib,"D3DCore.lib")

using namespace space;

LRESULT WINAPI WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT ps;
	HDC hdc;

	int width, height;
	int x, y;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	try
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = TEXT("RenderWindow");
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)NULL);
		if (!RegisterClassEx(&wcex)){
			return -1;
		}

		HWND hWnd = CreateWindow(wcex.lpszClassName, TEXT("MainWindow"), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, nullptr, nullptr, hInstance, nullptr);
		ShowWindow(hWnd, true);

		D3DCore* core = D3DCore::Constructor();

		core->Initialize(hWnd);

		// Main message loop
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				core->Render();
			}
		}

	}
	catch (std::exception& e)
	{
		printf("%s", e.what());
	}

	return 0;
}

