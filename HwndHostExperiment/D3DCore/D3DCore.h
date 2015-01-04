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
		static D3DCore* Constructor();

		virtual ~D3DCore();
		virtual void Initialize(HWND handle) = 0;
		virtual void Render() = 0;
	};
}

extern "C"
{
	D3DCORE_API space::D3DCore* D3DCore_Constructor();
	D3DCORE_API void D3DCore_Initialize(space::D3DCore* pThis, HWND handle);
	D3DCORE_API void D3DCore_Render(space::D3DCore* pThis);
}