#include <assert.h>
#include <list>
#include <vector>
#include <mutex>
#include <atlbase.h>
#include <Windows.h>
#include <windowsx.h>
#include <d3dcompiler.h> 

#include <DirectXMath.h>
using namespace DirectX;

#include "D3DLayeredWindow.h"

class D3DWindowCore
{
private:
	D3DWindowCore()
	{
		CComPtr<IDXGIFactory> pDXGIFactory = nullptr;
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);

		CComPtr<IDXGIAdapter> pAdapter = nullptr;
		std::vector<CComPtr<IDXGIAdapter>> pAdapters;
		int i = 0;
		while (pDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			pAdapters.push_back(pAdapter);
			i++;
			pAdapter = nullptr;
		}

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		D3D_DRIVER_TYPE                     driverType = D3D_DRIVER_TYPE_UNKNOWN;
		D3D_FEATURE_LEVEL                   featureLevel = D3D_FEATURE_LEVEL_11_0;

		for (UINT i = 0; i < pAdapters.size(); ++i){
			pDevice = nullptr; pImmediateContext = nullptr;
			hr = D3D11CreateDevice(pAdapters.at(i), driverType, NULL, createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&pDevice,
				&featureLevel,
				&pImmediateContext);

			if (SUCCEEDED(hr) && featureLevel >= D3D_FEATURE_LEVEL_11_0)
			{
				break;
			}
		}
		if (FAILED(hr))
		{
			return;
		}

		currentThreadId = GetCurrentThreadId();
	}

	void InitBuffer()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.StructureByteStride = sizeof(XMFLOAT3);
		desc.ByteWidth = 4 * sizeof(XMFLOAT3);
		//desc.CPUAccessFlags = 0;
		//desc.MiscFlags = 0;

		XMFLOAT3 buffer[] =
		{
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f },
		};

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = buffer;

		HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pVertexBuffer);
		if (FAILED(hr))
		{
			throw std::exception("CreateBuffer failed.");
		}
		pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		ID3D11Buffer* buffers[] =
		{
			pVertexBuffer
		};
		UINT strides[] =
		{
			sizeof(XMFLOAT3)
		};
		UINT offsets[] =
		{
			0
		};
		pImmediateContext->IASetVertexBuffers(0, 1,buffers,strides,offsets);

		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = 4 * sizeof(int);

		hr = pDevice->CreateBuffer(&desc, nullptr, &pShaderResource);

	}

	void InitShader()
	{
		//Compile Vertex Shader
		CComPtr<ID3DBlob> pCodeBlob = nullptr;
		CComPtr<ID3DBlob> pErrorBlob = nullptr;
		HRESULT hr = D3DCompile(
			strVertexShaderCode.c_str(), strVertexShaderCode.size(), "VertexShader",
			nullptr, nullptr, "main", "vs_5_0",
#if defined _DEBUG || defined DEBUG
			D3DCOMPILE_DEBUG
#else
			D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif		
			,0,
			&pCodeBlob,&pErrorBlob
			);

		if (FAILED(hr))
		{
			std::string error((char*)pErrorBlob->GetBufferPointer());
			throw std::exception(("D3DCompile Vertex Shader failed." + error).c_str());
		}
		
		pVertexShader = nullptr;
		hr = pDevice->CreateVertexShader(pCodeBlob->GetBufferPointer(), pCodeBlob->GetBufferSize(), nullptr, &pVertexShader);
		if (FAILED(hr))
		{
			throw std::exception("CreateVertexShader failed.");
		}

		//Compile Pixel Shader
		pCodeBlob = nullptr;
		pErrorBlob = nullptr;
		hr = D3DCompile(
			strPixelShaderCode.c_str(), strPixelShaderCode.size(), "PixelShader",
			nullptr, nullptr, "main", "ps_5_0",
#if defined _DEBUG || defined DEBUG
			D3DCOMPILE_DEBUG
#else
			D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif		
			, 0,
			&pCodeBlob, &pErrorBlob
			);
		
		if (FAILED(hr))
		{
			std::string error((char*)pErrorBlob->GetBufferPointer());
			throw std::exception(("D3DCompile Pixel Shader failed." + error).c_str());
		}
		pPixelShader = nullptr;
		hr = pDevice->CreatePixelShader(pCodeBlob->GetBufferPointer(), pCodeBlob->GetBufferSize(), nullptr, &pPixelShader);
		if (FAILED(hr))
		{
			throw std::exception("CreatePixelShader failed.");
		}

		//Set Shaders
		pImmediateContext->VSSetShader(pVertexShader, nullptr, 0);
		pImmediateContext->PSSetShader(pPixelShader, nullptr, 0);
	}

	DWORD currentThreadId = NULL;

	static const std::string strVertexShaderCode;
	static const std::string strPixelShaderCode;

	static std::once_flag flag;
	static std::unique_ptr<D3DWindowCore> instance;

	CComPtr<ID3D11Device> pDevice = nullptr;
	CComPtr<ID3D11DeviceContext> pImmediateContext = nullptr;

	CComPtr<ID3D11Buffer>	pVertexBuffer = nullptr;
	CComPtr<ID3D11Buffer>	pShaderResource = nullptr;

	CComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	CComPtr<ID3D11PixelShader>	pPixelShader = nullptr;
	
	ID3D11Texture2D* pTarget = nullptr;
	CComPtr<ID3D11RenderTargetView> pRenderTargetView = nullptr;
	int sizeX = 0, sizeY = 0;
public:
	~D3DWindowCore()
	{

	}
	static D3DWindowCore* GetInstance()
	{
		std::call_once(flag,[]()
		{
			instance.reset(new D3DWindowCore);
		});
		return instance.get();
	}

	ID3D11Device* GetDevice() const
	{
		return pDevice.p;
	}

	ID3D11DeviceContext* GetImmediateContext() const
	{
		return pImmediateContext.p;
	}

	void SetTarget(ID3D11Texture2D* pTarget)
	{
		if (pTarget == this->pTarget)
		{
			return;
		}
		else
		{
			this->pTarget = pTarget;
		}

		D3D11_TEXTURE2D_DESC texDesc;
		pTarget->GetDesc(&texDesc);
		
		sizeX = texDesc.Width; sizeY = texDesc.Height;

		pRenderTargetView = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = texDesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 1;
		HRESULT hr = pDevice->CreateRenderTargetView(pTarget,&desc , &pRenderTargetView);
		if (FAILED(hr))
		{
			throw std::exception("CreateRenderTargetView failed.");
		}

		ID3D11RenderTargetView* targets[] = 
		{
			pRenderTargetView
		};
		GetImmediateContext()->OMSetRenderTargets(1,targets, nullptr);

		D3D11_VIEWPORT vps[] = {
			{ 0,0,texDesc.Width,texDesc.Height,0,1 }
		};		
		GetImmediateContext()->RSSetViewports(1, vps);

		float const clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetImmediateContext()->ClearRenderTargetView(pRenderTargetView, clearColor);
	}

	void DrawSubWindow(ID3D11Texture2D* pBackBuffer,int posX,int posY,int sizeX,int sizeY) const
	{
		if (pBackBuffer == nullptr)
			return;
		
		D3D11_TEXTURE2D_DESC texDesc;
		pBackBuffer->GetDesc(&texDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = texDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2D.MostDetailedMip = 0;

		CComPtr<ID3D11ShaderResourceView> pShaderResourceView = nullptr;
		HRESULT hr =GetDevice()->CreateShaderResourceView(pBackBuffer, &desc, &pShaderResourceView);
		if (FAILED(hr))
		{
			throw std::exception("CreateShaderResourceView failed.");
		}
		int pos[4] = { posX, posY, sizeX, sizeY };
		GetImmediateContext()->UpdateSubresource(pShaderResource, 1, 0, pos, 4 * sizeof(int), NULL);

		ID3D11Buffer* buffers[] =
		{
			pShaderResource
		};
		GetImmediateContext()->VSSetConstantBuffers(0, 1, buffers);
		ID3D11ShaderResourceView* SRVs[] =
		{
			pShaderResourceView
		};
		GetImmediateContext()->PSSetShaderResources(0, 1, SRVs);
		
		GetImmediateContext()->Draw(4, 0);
	}

	bool IsSameThread(DWORD threadId) const
	{
		return threadId == currentThreadId;
	} 
};

#define AssertCurrentThread() \
	assert(D3DWindowCore::GetInstance()->IsSameThread(GetCurrentThreadId()))


std::once_flag D3DWindowCore::flag;
std::unique_ptr<D3DWindowCore> D3DWindowCore::instance;

const std::string D3DWindowCore::strVertexShaderCode =
"\
int top,left,bottom,right,sizeX,sizeY;\
void main(in float3 input : POSITION0,out float3 output : SV_Position, out float2 screenPos :TEXCOORD0)\
{\
	float scaleX = (right - left) / sizeX;\
	float scaleY = (bottom - top) / sizeY;\
	matrix<float,4,4> transform = \
		{ scaleX,	0.0,		0.0,	(float)left/sizeX,\
		  0.0,		scaleY,		0.0,	(float)top/sizeY,\
		  0.0,		0.0,		1.0,	0.0,\
		  0.0,		0.0,		0.0,	1.0 };\
	output = mul(input, transform);\
	screenPos = input.xy;\
}\
";
const std::string D3DWindowCore::strPixelShaderCode =
"\
Texture backbuffer;\
SampleState sampleState;\
void main(in float3 input : SV_Position,in float2 screenPos : TEXCOORD0, out float4 OutColor : SV_Target)\
{\
	OutColor = backbuffer.Sample(sampleState,screenPos);\
}\
";

class D3DWindowBuffer::Impl
{
public:
	CComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	Impl()
	{

	}
	~Impl()
	{
	}
	void SetBuffer(ID3D11Texture2D* pBuffer)
	{
		if (pBuffer == nullptr)
		{
			return ;
		}

		D3D11_TEXTURE2D_DESC desc;
		pBuffer->GetDesc(&desc);

		if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM
			|| desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
			|| desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM
			|| desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
		{
			return;
		}
		
		if (desc.MiscFlags & D3D11_RESOURCE_MISC_SHARED == 0)
		{
			return;
		}

		CComPtr<IDXGIResource> pResource = nullptr;
		HRESULT hr = pBuffer->QueryInterface(__uuidof(IDXGIResource),(void**)&pResource);
		if (FAILED(hr))
		{
			throw std::exception("QueryInterface IDXGIResource failed.");
		}

		HANDLE hShared = NULL;
		hr = pResource->GetSharedHandle(&hShared);
		auto core = D3DWindowCore::GetInstance();

		CComPtr<ID3D11Texture2D> pNewBuffer = nullptr;
		hr = core->GetDevice()->OpenSharedResource(hShared, __uuidof(ID3D11Texture2D),(void**)&pNewBuffer);
		if (FAILED(hr))
		{
			throw std::exception("OpenSharedResource failed.");
		} 
		this->pBackBuffer = pNewBuffer;
	}
	ID3D11Texture2D* GetBuffer() const
	{
		return pBackBuffer;
	}  
};

D3DWindowBuffer::D3DWindowBuffer(ID3D11Texture2D* pBuffer)
	:impl(new Impl()){ 
	impl->SetBuffer(pBuffer);
}

D3DWindowBuffer::D3DWindowBuffer()
	: impl(new Impl()){
}

ID3D11Texture2D* D3DWindowBuffer::GetBuffer() const
{
	return impl->GetBuffer();
}

void D3DWindowBuffer::SetBuffer(ID3D11Texture2D* pBuffer)
{
	impl->SetBuffer(pBuffer);
}

class D3DWindowFrame::Impl
{
private:	
	D3DWindowBuffer ret;
	
	int sizeX, sizeY;

	void RecreateBuffer(int X, int Y)
	{
		auto core = D3DWindowCore::GetInstance();

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc)); 
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		desc.Width = X;
		desc.Height = Y;
		desc.MipLevels = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.MiscFlags = 0;
		CComPtr<ID3D11Texture2D> pNewBuffer = nullptr;
		HRESULT hr = core->GetDevice()->CreateTexture2D(&desc, nullptr, &pNewBuffer);
		if (FAILED(hr))
		{
			throw std::exception("D3DWindowFrame::Impl::RecreateBuffer() CreateTexture2D failed.");
		}
		ret.impl->pBackBuffer = pNewBuffer;
	}
public:
	Impl(int X, int Y):sizeX(X),sizeY(Y)
	{
		RecreateBuffer(sizeX, sizeY);
	}
	~Impl()
	{}

	D3DWindowBuffer* GetBackBuffer() const
	{ 
		return (D3DWindowBuffer*)&ret;
	}

	int GetSizeX() const
	{
		return sizeX;
	}

	int GetSizeY() const
	{
		return sizeY;
	}

	void Resize(int X, int Y)
	{
		sizeX = X, sizeY = Y;
		RecreateBuffer(sizeX, sizeY);
	}

	void DrawSubWindow(D3DWindowBuffer * pSubWindow, int posX, int posY, int sizeX, int sizeY)
	{
		auto core = D3DWindowCore::GetInstance();

		core->DrawSubWindow(pSubWindow->GetBuffer(), posX, posY, sizeX, sizeY);
	}

	void BeginDraw()
	{
		auto core = D3DWindowCore::GetInstance();
		
		core->SetTarget(ret.GetBuffer());
	}

	void EndDraw()
	{
		auto core = D3DWindowCore::GetInstance();

		core->GetImmediateContext()->Flush();
	}

};


D3DWindowFrame::D3DWindowFrame(int X,int Y)
	:impl(new Impl(X,Y)){
}

D3DWindowFrame::~D3DWindowFrame()
{
}

int D3DWindowFrame::GetSizeX() const
{
	return impl->GetSizeX();
}

int D3DWindowFrame::GetSizeY() const
{
	return impl->GetSizeY();
}

void D3DWindowFrame::Resize(int X, int Y)
{
	impl->Resize(X, Y);
}

D3DWindowBuffer* D3DWindowFrame::GetBackBuffer() const
{
	return impl->GetBackBuffer();
}

void D3DWindowFrame::DrawSubWindow(D3DWindowBuffer* pSubWindow, int posX, int posY, int sizeX, int sizeY)
{
	impl->DrawSubWindow(pSubWindow, posX, posY, sizeX, sizeY);
}

void D3DWindowFrame::BeginDraw()
{
	impl->BeginDraw();
}

void D3DWindowFrame::EndDraw()
{
	impl->EndDraw();
}

class D3DLayeredWindow::Impl
{
private:
	HWND hWnd;
	HDC hScreen;
	HDC hDC;

	HBITMAP hBitmap;

	int sizeX, sizeY;
	std::unique_ptr<D3DWindowFrame> pFrame = nullptr; 

	static LRESULT WINAPI MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_CREATE)
		{
			SetWindowLong(hWnd, 0, (LONG)lParam);
		}
		else
		{
			Impl* pThis = (Impl*)GetWindowLong(hWnd, 0);
			if (pThis != nullptr)
			{
				return pThis->WndProc(hWnd,message,wParam,lParam);
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	
public:
	Impl(int sizeX, int sizeY) 
		:sizeX(sizeX), sizeY(sizeY)
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MessageRouter;
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
			throw std::exception("RegisterClassEx failed.");
		}

		hWnd = CreateWindowEx(WS_EX_LAYERED,wcex.lpszClassName, TEXT("MainWindow"), WS_OVERLAPPEDWINDOW,
			0, 0, sizeX, sizeY, nullptr, nullptr, hInstance,(LPVOID)this);
		
		ShowWindow(hWnd, false);

		pFrame.reset(new D3DWindowFrame(sizeX,sizeY));
		hScreen = GetDC(hWnd);

		hDC = CreateCompatibleDC(hScreen);

		hBitmap = CreateCompatibleBitmap(hDC, sizeX, sizeY);
	}
	~Impl()
	{ 
		if (hScreen != NULL)
			ReleaseDC(hWnd, hScreen);
		if (hDC != NULL)
			DeleteDC(hDC);
		if (hBitmap != NULL)
			DeleteBitmap(hBitmap);
	} 

	D3DWindowFrame* GetFrame()
	{
		return pFrame.get();
	}

	void Resize(int X, int Y)
	{
		sizeX = X, sizeY = Y;
		pFrame->Resize(X, Y);

		::SetWindowPos(hWnd, NULL, 0, 0, X, Y, 0);

		Update();
	}

	int GetSizeX() const
	{
		return sizeX;
	}

	int GetSizeY() const
	{
		return sizeY;
	}

	void SetWindowPos(int top, int left)
	{
		::SetWindowPos(hWnd, NULL, top, left, sizeX, sizeY, 0);
	}

	void Update()
	{
		auto core = D3DWindowCore::GetInstance();

		auto pBuffer = pFrame->GetBackBuffer()->GetBuffer();
		D3D11_TEXTURE2D_DESC texDesc;
		pBuffer->GetDesc(&texDesc);
		texDesc.BindFlags = 0;
		texDesc.Usage = D3D11_USAGE_STAGING;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		CComPtr<ID3D11Texture2D> pStagingBuffer = nullptr;
		HRESULT hr = core->GetDevice()->CreateTexture2D(&texDesc, nullptr, &pStagingBuffer);
		if (FAILED(hr))
		{
			throw std::exception("CreateTexture2D failed.");
		}

		core->GetImmediateContext()->CopyResource(pStagingBuffer, pBuffer);

		D3D11_MAPPED_SUBRESOURCE data;
		hr = core->GetImmediateContext()->Map(pStagingBuffer, 0, D3D11_MAP_READ, 0, &data);

		SetBitmapBits(hBitmap, data.DepthPitch * texDesc.Height , data.pData);
				
		core->GetImmediateContext()->Unmap(pStagingBuffer, 0);
		
		SelectObject(hDC, hBitmap);

		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;
		POINT ptPos = { 0, 0 };
		SIZE sizeWnd = { sizeX , sizeY };
		POINT ptSrc = { 0, 0 };
		UpdateLayeredWindow(hWnd, hScreen,&ptPos,&sizeWnd,hDC,&ptSrc,0,&blend,ULW_ALPHA);
	}
};

D3DLayeredWindow::D3DLayeredWindow(int sizeX,int sizeY)
	:impl(new Impl(sizeX,sizeY)){
}
D3DLayeredWindow::~D3DLayeredWindow()
{
}
D3DLayeredWindow::D3DLayeredWindow(D3DLayeredWindow const& other)
{ 
}
D3DLayeredWindow& D3DLayeredWindow::operator=(D3DLayeredWindow const& other)
{
	return *this;
}
D3DWindowFrame* D3DLayeredWindow::GetFrame()
{
	return impl->GetFrame();
}
void D3DLayeredWindow::Update()
{
	impl->Update();
}

void D3DLayeredWindow::Resize(int X, int Y)
{
	impl->Resize(X, Y);
} 
int D3DLayeredWindow::GetSizeX() const
{
	return impl->GetSizeX();
}

int D3DLayeredWindow::GetSizeY() const
{
	return impl->GetSizeY();
}
void D3DLayeredWindow::SetWindowPos(int top, int left)
{
	return impl->SetWindowPos(top, left);
}
//=============
// D3DWindowBuffer C style interface implement
//=============
D3DWindowBuffer* D3DWindowBuffer_Constructor()
{
	try
	{
		return new D3DWindowBuffer();
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
		return nullptr;
	}
}
D3DWindowBuffer* D3DWindowBuffer_Constructor2(ID3D11Texture2D* pBuffer){
	try
	{
		return new D3DWindowBuffer(pBuffer);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
		return nullptr;
	}
}
void D3DWindowBuffer_Destructor(D3DWindowBuffer* pThis)
{
	if (pThis != nullptr)
		delete pThis;
}
void D3DWindowBuffer_SetBuffer(D3DWindowBuffer* pThis, ID3D11Texture2D* pBuffer)
{
	try
	{
		if (pThis != nullptr)
			pThis->SetBuffer(pBuffer);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
	}
}

//===================
// D3DWindowFrame C style interface implement
//===================
D3DWindowFrame* D3DWindowFrame_Constructor(int X, int Y){
	try
	{
		return new D3DWindowFrame(X,Y);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
		return nullptr;
	}
}
void D3DWindowFrame_Destructor(D3DWindowFrame* pThis)
{
	if (pThis != nullptr)
		delete pThis;
}
int D3DWindowFrame_GetSizeX(D3DWindowFrame* pThis)
{
	if (pThis != nullptr)
		return pThis->GetSizeX();
	return -1;
}
int D3DWindowFrame_GetSizeY(D3DWindowFrame* pThis)
{
	if (pThis != nullptr)
		return pThis->GetSizeY();
	return -1;
}
void D3DWindowFrame_Resize(D3DWindowFrame* pThis, int X, int Y)
{
	try
	{
		if (pThis != nullptr)
			return pThis->Resize(X, Y);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
	}
}
D3DWindowBuffer* D3DWindowFrame_GetBackBuffer(D3DWindowFrame* pThis)
{
	if (pThis != nullptr)
		return pThis->GetBackBuffer();
	return nullptr;
}
void D3DWindowFrame_DrawSubWindow(D3DWindowFrame* pThis, D3DWindowBuffer* pSubWindow, int posX, int posY, int sizeX, int sizeY)
{
	try
	{

		if (pThis != nullptr)
			return pThis->DrawSubWindow(pSubWindow, posX, posY, sizeX, sizeY);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
	}
}
void D3DWindowFrame_BeginDraw(D3DWindowFrame* pThis)
{
	if (pThis != nullptr)
	{
		pThis->BeginDraw();
	}
}

void D3DWindowFrame_EndDraw(D3DWindowFrame* pThis)
{
	if (pThis != nullptr)
	{
		pThis->EndDraw();
	}
}

//=========================
// D3DLayeredWindow C style interface implement
//=========================
D3DLayeredWindow* D3DLayeredWindow_Constructor(int sizeX, int sizeY)
{
	try
	{
		return new D3DLayeredWindow(sizeX, sizeY);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
		return nullptr;
	}
}
void D3DLayeredWindow_Destructor(D3DLayeredWindow* pThis)
{
	if (pThis != nullptr)
		delete pThis;
}

void D3DLayeredWindow_Update(D3DLayeredWindow* pThis)
{
	try
	{
		if(pThis != nullptr)
			pThis->Update();
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
	}
}
D3DWindowFrame* D3DLayeredWindow_GetFrame(D3DLayeredWindow* pThis)
{
	if (pThis != nullptr)
		return pThis->GetFrame();
}
void D3DLayeredWindow_Resize(D3DLayeredWindow* pThis, int X, int Y)
{
	try
	{
		if (pThis != nullptr)
			pThis->Resize(X, Y);
	}
	catch (std::exception &e)
	{
		OutputDebugStringA(e.what());
	}
}
int D3DLayeredWindow_GetSizeX(D3DLayeredWindow* pThis)
{
	if (pThis != nullptr)
		return pThis->GetSizeX();
}
int D3DLayeredWindow_GetSizeY(D3DLayeredWindow* pThis)
{
	if (pThis != nullptr)
		return pThis->GetSizeY();
}
void D3DLayeredWindow_SetWindowPos(D3DLayeredWindow* pThis, int top, int left)
{
	if (pThis != nullptr)
		pThis->SetWindowPos(top, left);
}