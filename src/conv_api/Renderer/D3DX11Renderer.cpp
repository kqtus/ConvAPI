#include "D3DX11Renderer.h"
#include "IRenderable.h"
#include "IRenderSource.h"
#include "ID3DXRenderData.h"

#include <Windows.h>

using namespace DirectX;

CD3DX11Renderer::CD3DX11Renderer(HWND target_window)
	: m_WindowHandler(target_window)
{
	XMMATRIX identity = XMMatrixIdentity();
	XMStoreFloat4x4(&m_View, identity);
	XMStoreFloat4x4(&m_Proj, identity);
}

CD3DX11Renderer::~CD3DX11Renderer()
{
	// #TODO: Release all COM objects;
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
	BuildFX();
	BuildVertexLayout();

	return true;
}

void CD3DX11Renderer::Update(float dt)
{
	float x = m_Radius * sinf(m_Phi) * cosf(m_Theta);
	float y = m_Radius * sinf(m_Phi) * sinf(m_Phi);
	float z = m_Radius * cosf(m_Phi);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX v = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m_View, v);
}

void CD3DX11Renderer::Render()
{
	XMVECTORF32 blue = { 0.f, 0.f, 0.f, 1.f };

	m_D3DContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&blue));
	m_D3DContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_D3DContext->IASetInputLayout(m_InputLayout);

	{
		m_D3DContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_D3DContext->VSSetShader(m_BgVertShader, nullptr, 0);
		m_D3DContext->PSSetShader(m_BgPixelShader, nullptr, 0);
		m_D3DContext->Draw(4, 0);
	}


	m_D3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC tech_desc;
	m_Technique->GetDesc(&tech_desc);

	XMMATRIX view = XMLoadFloat4x4(&m_View);
	XMMATRIX proj = XMLoadFloat4x4(&m_Proj);

	UINT stride = sizeof(IRenderable::TVertex);
	UINT offset = 0;

	for (auto& render_src : m_RenderSources)
	{
		for (auto& src : m_RenderSources)
		{
			IRenderSource::TRenderables renderables;
			src->GetRenderables(renderables);

			for (auto& renderable : renderables)
			{
				if (auto d3d_renderable = dynamic_cast<ID3DXRenderData*>(renderable))
				{
					const XMMATRIX& world = XMLoadFloat4x4(&d3d_renderable->GetWorldMat());
					XMMATRIX wvp_mat = world * view * proj;
					m_FxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&wvp_mat));

					auto vbo = d3d_renderable->GetVertexBuffer();
					auto ibo = d3d_renderable->GetIndexBuffer();

					m_D3DContext->IASetVertexBuffers(0, 1, &vbo, &stride, &offset);
					m_D3DContext->IASetIndexBuffer(ibo, DXGI_FORMAT_R32_UINT, 0);

					for (UINT i = 0; i < tech_desc.Passes; ++i)
					{
						m_Technique->GetPassByIndex(i)->Apply(0, m_D3DContext);
						m_D3DContext->DrawIndexed(renderable->GetIndexCount(), 0, 0);
					}
				}
			}
		}
	}

	m_SwapChain->Present(0, 0);
}

template<class T>
T _clamp(T val, T min, T max)
{
	return val < min ? min : (val > max ? max : val);
}

void CD3DX11Renderer::Move(float dx, float dy)
{
	m_Radius += dx - dy;
	m_Radius = _clamp(m_Radius, 3.0f, 25.0f);
}

void CD3DX11Renderer::Rotate(float dx, float dy)
{
	m_Theta += dx;
	m_Phi += dy;
	m_Phi = _clamp(m_Phi, 0.1f, 3.14f - 0.1f);
}

void CD3DX11Renderer::OnResize()
{
	__super::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * 3.14f, 4.f / 3.f, 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_Proj, P);
}

bool CD3DX11Renderer::AddRenderSource(IRenderSource* render_src)
{
	if (__super::AddRenderSource(render_src))
	{
		IRenderSource::TRenderables renderables;
		render_src->GetRenderables(renderables);

		for (auto& renderable : renderables)
		{
			if (auto d3d_rendeable = dynamic_cast<ID3DXRenderData*>(renderable))
			{
				d3d_rendeable->InitBuffers(m_D3DDevice);
			}
		}

		return true;
	}
	return false;
}

bool CD3DX11Renderer::RemoveRenderSource(IRenderSource* render_src)
{
	if (__super::RemoveRenderSource(render_src))
	{
		IRenderSource::TRenderables renderables;
		render_src->GetRenderables(renderables);

		for (auto& renderable : renderables)
		{
			if (auto d3d_rendeable = dynamic_cast<ID3DXRenderData*>(renderable))
			{
				d3d_rendeable->ReleaseBuffers();
			}
		}

		return true;
	}
	return false;
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

void CD3DX11Renderer::BuildFX()
{
	DWORD shader_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shader_flags |= D3D10_SHADER_DEBUG;
	shader_flags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* compiled_shader = nullptr;
	ID3DBlob* compilation_msgs = nullptr;

	HRESULT hr = D3DCompileFromFile(L"C://Users//Marek//Documents//mod-dev//conv_api//fx//color.fx", nullptr, nullptr, nullptr, "fx_5_0", shader_flags, 0, &compiled_shader, &compilation_msgs);


	if (compilation_msgs)
	{
		//MessageBoxA(0, (char*)compilation_msgs->GetBufferPointer(), 0, 0);
		compilation_msgs->Release();
	}


	hr = D3DX11CreateEffectFromMemory(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), 0, m_D3DDevice, &m_Fx);

	if (FAILED(hr))
		return;

	m_Technique = m_Fx->GetTechniqueByName("ColorTech");
	m_FxWorldViewProj = m_Fx->GetVariableByName("gWorldViewProj")->AsMatrix();

	// Background.fx
	hr = D3DCompileFromFile(L"C://Users//Marek//Documents//mod-dev//conv_api//fx//background.fx", nullptr, nullptr, "main", "vs_5_0", 0, 0, &compiled_shader, &compilation_msgs);

	if (compilation_msgs)
	{
		MessageBoxA(0, (char*)compilation_msgs->GetBufferPointer(), 0, 0);
		compilation_msgs->Release();
	}

	if (FAILED(hr))
		return;

	hr = m_D3DDevice->CreateVertexShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &m_BgVertShader);

	hr = D3DCompileFromFile(L"C://Users//Marek//Documents//mod-dev//conv_api//fx//background.ps", nullptr, nullptr, "main", "ps_5_0", 0, 0, &compiled_shader, &compilation_msgs);

	if (compilation_msgs)
	{
		MessageBoxA(0, (char*)compilation_msgs->GetBufferPointer(), 0, 0);
		compilation_msgs->Release();
	}

	if (FAILED(hr))
		return;

	hr = m_D3DDevice->CreatePixelShader(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), nullptr, &m_BgPixelShader);

	compiled_shader->Release();
}

void CD3DX11Renderer::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3DX11_PASS_DESC pass_desc;
	m_Technique->GetPassByIndex(0)->GetDesc(&pass_desc);

	m_D3DDevice->CreateInputLayout(vertex_desc, 2, pass_desc.pIAInputSignature, pass_desc.IAInputSignatureSize, &m_InputLayout);
}