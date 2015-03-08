#include "D3DCore.h"

#include <vector>
#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <memory>
#include <mutex>
#include <wrl\client.h>
using namespace Microsoft::WRL;

namespace space
{
	class D3DRendererImpl : public D3DCore
	{
	private:

		ComPtr<IDXGIFactory> pDXGIFactory;
		ComPtr<ID3D11Device> pD3DDevice;
		ComPtr<ID3D11DeviceContext> pDeviceContext;

		ComPtr<IDXGISwapChain> pSwapChain;
		ComPtr<ID3D11Texture2D> pBuffer;
		ComPtr<ID3D11RenderTargetView> pRenderTarget;

	public:
		D3DRendererImpl()
		{
			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf());
			
			ComPtr<IDXGIAdapter> pAdapter;
			std::vector<ComPtr<IDXGIAdapter>> pAdapters;
			int i = 0;
			while (pDXGIFactory->EnumAdapters(i, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
			{
				pAdapters.push_back(pAdapter);
				i++;
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
				hr = D3D11CreateDevice(pAdapters.at(i).Get(), driverType, NULL, createDeviceFlags,
					featureLevels,
					numFeatureLevels,
					D3D11_SDK_VERSION,
					pD3DDevice.GetAddressOf(),
					&featureLevel,
					pDeviceContext.GetAddressOf());
			
				if (SUCCEEDED(hr) && featureLevel >= D3D_FEATURE_LEVEL_11_0)
				{
					break;
				}
			}
			if (FAILED(hr))
			{
				return;
			}
		}

		~D3DRendererImpl()
		{}

		virtual void Initialize(HWND handle) override
		{
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;
			sd.BufferDesc.Width = 500;
			sd.BufferDesc.Height = 500;
			sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.OutputWindow = handle;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.Windowed = TRUE;

			HRESULT hr = pDXGIFactory->CreateSwapChain(pD3DDevice.Get(), &sd,pSwapChain.GetAddressOf());
			if (FAILED(hr))
			{
				return ;
			}
			
			if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBuffer.GetAddressOf())))
			{
				pD3DDevice->CreateRenderTargetView(pBuffer.Get(),nullptr, pRenderTarget.GetAddressOf());
			}			
		}

		virtual void Render() override
		{
			ID3D11RenderTargetView* targets[] = { pRenderTarget.Get() };			
			pDeviceContext->OMSetRenderTargets(1, targets, nullptr);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			D3D11_VIEWPORT viewPorts[]
			{
				{0, 0, sd.BufferDesc.Width / 2, sd.BufferDesc.Height, 0, 1}				
			};
			D3D11_RECT rects[]
			{
				{0, 0, sd.BufferDesc.Width / 2, sd.BufferDesc.Height }
			};
			pDeviceContext->RSSetViewports(1, viewPorts);
			pDeviceContext->RSSetScissorRects(1, rects);
			float clearColor[] = { 0.0f, 0.3f, 0.5f, 0.0f };
			pDeviceContext->ClearRenderTargetView(pRenderTarget.Get(), clearColor);
			viewPorts[0] = { 0, sd.BufferDesc.Height / 2, sd.BufferDesc.Width / 2, sd.BufferDesc.Height, 0, 1 }
			;
			pDeviceContext->RSSetViewports(1, viewPorts);

			float clearColor2[] = { 0.8f, 0.3f, 0.1f, 0.0f };
			pDeviceContext->ClearRenderTargetView(pRenderTarget.Get(), clearColor2);

			pSwapChain->Present(0, 0);
		}
	};

	D3DCore::D3DCore()
	{}
	D3DCore::~D3DCore()
	{}

	static std::unique_ptr<D3DCore> instance;
	static std::once_flag coreFlag;
	D3DCore* D3DCore::CreateInstance()
	{
		try
		{
			return new D3DRendererImpl();
		}
		catch (std::exception &e)
		{
			return nullptr;
		}
	}
	
}
