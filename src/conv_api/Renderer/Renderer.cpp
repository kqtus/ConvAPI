#include "Renderer.h"

void CRenderer::SetWindowWidth(UINT width)
{
	m_ClientWidth = width;
	OnWindowWidthSet();
}

void CRenderer::SetWindowHeight(UINT height)
{
	m_ClientHeight = height;
	OnWindowHeightSet();
}

UINT CRenderer::GetWindowWidth() const
{
	return m_ClientWidth;
}

UINT CRenderer::GetWindowHeight() const
{
	return m_ClientHeight;
}

#include <Windows.h>

CD3DX11Renderer::CD3DX11Renderer(HWND target_window)
	: m_WindowHandler(target_window)
{
}

CD3DX11Renderer::~CD3DX11Renderer()
{

}

bool CD3DX11Renderer::Init()
{
	if (!CreateDevice())
		return false;

	FillSwapChainDesc();

	if (!CreateSwapChain())
		return false;

	if (!CreateRenderTargetView())
		return false;

	if (!CreateDepthStencil())
		return false;

	CreateViewport();

	return true;
}

void CD3DX11Renderer::Update(float dt)
{

}

void CD3DX11Renderer::Render()
{
	XMVECTORF32 blue = { 0.f, 0.f, 1.f, 1.f };

	m_D3DContext->ClearRenderTargetView(
		m_RenderTargetView,
		reinterpret_cast<const float*>(&blue)
	);

	
	m_D3DContext->ClearDepthStencilView(
		m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
	

	m_SwapChain->Present(0, 0);
}

HWND CD3DX11Renderer::GetWindowHandler()
{
	return m_WindowHandler;
}

bool CD3DX11Renderer::CreateDevice()
{
	UINT create_device_flags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL feature_level;

	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		create_device_flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&m_D3DDevice,
		&feature_level,
		&m_D3DContext
	);

	if (FAILED(result))
	{
		return false;
	}

	if (feature_level != D3D_FEATURE_LEVEL_11_0)
	{
		return false;
	}

	return true;
}

void CD3DX11Renderer::FillSwapChainDesc()
{
	HRESULT result = m_D3DDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&m_Msaa4xQuality
	);

	DXGI_SWAP_CHAIN_DESC& sd = m_SwapChainDesc;

	sd.BufferDesc.Width = GetWindowWidth();
	sd.BufferDesc.Height = GetWindowHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_Msaa4xQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_WindowHandler;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
}

bool CD3DX11Renderer::CreateSwapChain()
{
	HRESULT result;
	IDXGIDevice* dxgi_device = nullptr;
	result = m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);

	if (FAILED(result))
		return false;

	IDXGIAdapter* dxgi_adapter = nullptr;
	result = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);

	if (FAILED(result))
		return false;

	IDXGIFactory* dxgi_factory = nullptr;
	result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);

	if (FAILED(result))
		return false;

	result = dxgi_factory->CreateSwapChain(m_D3DDevice, &m_SwapChainDesc, &m_SwapChain);

	if (FAILED(result))
		return false;

	dxgi_device->Release();
	dxgi_adapter->Release();
	dxgi_factory->Release();

	return true;
}

bool CD3DX11Renderer::CreateRenderTargetView()
{
	ID3D11Texture2D* back_buffer = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	m_D3DDevice->CreateRenderTargetView(back_buffer, 0, &m_RenderTargetView);

	back_buffer->Release();
	return true;
}

bool CD3DX11Renderer::CreateDepthStencil()
{
	D3D11_TEXTURE2D_DESC ds_desc;
	ds_desc.Width = GetWindowWidth();
	ds_desc.Height = GetWindowHeight();
	ds_desc.MipLevels = 1;
	ds_desc.ArraySize = 1;
	ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (m_Enable4xMsaa)
	{
		ds_desc.SampleDesc.Count = 4;
		ds_desc.SampleDesc.Quality = m_Msaa4xQuality - 1;
	}
	else
	{
		ds_desc.SampleDesc.Count = 1;
		ds_desc.SampleDesc.Quality = 0;
	}

	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.MiscFlags = 0;

	HRESULT result = m_D3DDevice->CreateTexture2D(&ds_desc, 0, &m_DepthStencilBuffer);

	if (FAILED(result))
		return false;

	result = m_D3DDevice->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView);

	if (FAILED(result))
		return false;

	m_D3DContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	return true;
}

void CD3DX11Renderer::CreateViewport()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width = (float)GetWindowWidth();
	vp.Height = (float)GetWindowHeight();
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;

	m_D3DContext->RSSetViewports(1, &vp);
}