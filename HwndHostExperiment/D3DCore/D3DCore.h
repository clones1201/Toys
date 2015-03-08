#pragma once

#ifdef D3DCORE_EXPORTS
#define D3DCORE_API __declspec(dllexport)
#else
#define D3DCORE_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <windowsx.h>

namespace space
{
	class D3DCORE_API D3DCore
	{
	protected:
		D3DCore();
	public:
		static D3DCore* CreateInstance();

		virtual ~D3DCore();
		virtual void Initialize(HWND handle) = 0;
		virtual void Render() = 0;
	};
}

extern "C"
{
	 
}