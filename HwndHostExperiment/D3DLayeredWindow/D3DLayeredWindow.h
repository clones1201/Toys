#ifndef __D3DLAYEREDWINDOW_H__
#define __D3DLAYEREDWINDOW_H__

#ifdef D3DLAYEREDWINDOW_EXPORTS
#define D3DLAYEREDWINDOW_API __declspec(dllexport)
#else
#define D3DLAYEREDWINDOW_API __declspec(dllimport)
#endif

#include <d3d11.h>

#include <memory>

class D3DLAYEREDWINDOW_API D3DWindowBuffer
{
private:
	class Impl;
	std::unique_ptr<Impl> impl;

	ID3D11Texture2D* GetBuffer() const;
public:
	D3DWindowBuffer(ID3D11Texture2D* pBuffer);
	D3DWindowBuffer();

	void SetBuffer(ID3D11Texture2D* pBuffer);

	friend class D3DWindowFrame;
	friend class D3DWindowCore;
	friend class D3DLayeredWindow;
};

class D3DLAYEREDWINDOW_API D3DWindowFrame
{
private:
	class Impl;
	std::unique_ptr<Impl> impl; 

public:
	D3DWindowFrame(int X,int Y);
	~D3DWindowFrame();
	
	int GetSizeX() const;
	int GetSizeY() const;
	void Resize(int sizeX, int sizeY);
		
	D3DWindowBuffer* GetBackBuffer() const;
	 
	void BeginDraw();
	void EndDraw();
	void DrawSubWindow(D3DWindowBuffer * pSubWindow, int posX, int posY, int sizeX, int sizeY);

	friend class D3DWindowCore;
};

class D3DLAYEREDWINDOW_API D3DLayeredWindow
{
private:
	class Impl;
	std::unique_ptr<Impl> impl;
	D3DLayeredWindow(D3DLayeredWindow const& other);
	D3DLayeredWindow& operator=(D3DLayeredWindow const& other);
public:
	D3DLayeredWindow(int sizeX,int sizeY);
	~D3DLayeredWindow();

	D3DWindowFrame* GetFrame();

	void Resize(int X, int Y);
	int GetSizeX() const;
	int GetSizeY() const;
	void SetWindowPos(int top, int left);

	void Update();
};

extern "C"
{
	D3DLAYEREDWINDOW_API D3DWindowBuffer* D3DWindowBuffer_Constructor();
	D3DLAYEREDWINDOW_API D3DWindowBuffer* D3DWindowBuffer_Constructor2(ID3D11Texture2D* pBuffer);
	D3DLAYEREDWINDOW_API void D3DWindowBuffer_Destructor(D3DWindowBuffer* pThis);
	D3DLAYEREDWINDOW_API void D3DWindowBuffer_SetBuffer(D3DWindowBuffer* pThis, ID3D11Texture2D* pBuffer);
		
	D3DLAYEREDWINDOW_API D3DWindowFrame* D3DWindowFrame_Constructor(int X, int Y);
	D3DLAYEREDWINDOW_API void D3DWindowFrame_Destructor(D3DWindowFrame* pThis);
	D3DLAYEREDWINDOW_API int D3DWindowFrame_GetSizeX(D3DWindowFrame* pThis);
	D3DLAYEREDWINDOW_API int D3DWindowFrame_GetSizeY(D3DWindowFrame* pThis);
	D3DLAYEREDWINDOW_API void D3DWindowFrame_Resize(D3DWindowFrame* pThis, int X, int Y);
	D3DLAYEREDWINDOW_API D3DWindowBuffer* D3DWindowFrame_GetBackBuffer(D3DWindowFrame* pThis);
	D3DLAYEREDWINDOW_API void D3DWindowFrame_DrawSubWindow(D3DWindowFrame* pThis, D3DWindowBuffer* pSubWindow, int posX, int posY, int sizeX, int sizeY);
	D3DLAYEREDWINDOW_API void D3DWindowFrame_BeginDraw(D3DWindowFrame* pThis);
	D3DLAYEREDWINDOW_API void D3DWindowFrame_EndDraw(D3DWindowFrame* pThis);

	D3DLAYEREDWINDOW_API D3DLayeredWindow* D3DLayeredWindow_Constructor(int sizeX,int sizeY);
	D3DLAYEREDWINDOW_API void D3DLayeredWindow_Destructor(D3DLayeredWindow* pThis);
	D3DLAYEREDWINDOW_API D3DWindowFrame* D3DLayeredWindow_GetFrame(D3DLayeredWindow* pThis);
	D3DLAYEREDWINDOW_API void D3DLayeredWindow_Resize(D3DLayeredWindow* pThis,int X, int Y);
	D3DLAYEREDWINDOW_API int D3DLayeredWindow_GetSizeX(D3DLayeredWindow* pThis);
	D3DLAYEREDWINDOW_API int D3DLayeredWindow_GetSizeY(D3DLayeredWindow* pThis);
	D3DLAYEREDWINDOW_API void D3DLayeredWindow_SetWindowPos(D3DLayeredWindow* pThis, int top, int left);
	D3DLAYEREDWINDOW_API void D3DLayeredWindow_Update(D3DLayeredWindow* pThis);
}

#endif