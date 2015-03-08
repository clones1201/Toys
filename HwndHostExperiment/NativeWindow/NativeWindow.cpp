// NativeWindow.cpp : 定义控制台应用程序的入口点。
//

#include <Windows.h>

#include "D3DCore.h"
#include "D3DLayeredWindow.h"
  
int main(int argc, char* argv[])
{
	std::shared_ptr<D3DLayeredWindow> layeredWindow(new D3DLayeredWindow(500, 500));
	std::shared_ptr<space::D3DCore> core(space::D3DCore::CreateInstance());

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
			layeredWindow->Update();
		}
	}

	return 0;
}

