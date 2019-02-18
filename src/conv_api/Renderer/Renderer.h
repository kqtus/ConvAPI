#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

#pragma comment (lib, "D3D11.lib")

class CRenderer
{
public:
	CRenderer() { }
	virtual ~CRenderer() { }

	virtual bool Init() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	void SetWindowWidth(UINT width);
	void SetWindowHeight(UINT height);

	UINT GetWindowWidth() const;
	UINT GetWindowHeight() const;

protected:
	virtual void OnWindowWidthSet() { }
	virtual void OnWindowHeightSet() { }

	UINT m_ClientWidth = 640;
	UINT m_ClientHeight = 480;
};

class CD3DX11Renderer : public CRenderer
{
public:
	CD3DX11Renderer(HWND target_window);
	virtual ~CD3DX11Renderer();

	/* CRenderer Interface */
	virtual bool Init() override;

	virtual void Update(float dt) override;
	virtual void Render() override;
	/* End of CRenderer Interface */

	HWND GetWindowHandler();

protected:
	bool CreateDevice();

	void FillSwapChainDesc();
	bool CreateSwapChain();

	bool CreateRenderTargetView();
	bool CreateDepthStencil();

	void CreateViewport();

protected:
	ID3D11Device* m_D3DDevice = nullptr;
	ID3D11DeviceContext* m_D3DContext = nullptr;

	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
	IDXGISwapChain* m_SwapChain = nullptr;

	ID3D11RenderTargetView* m_RenderTargetView = nullptr;
	ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;

	bool m_Enable4xMsaa = false;
	UINT m_Msaa4xQuality = 0;

	HWND m_WindowHandler;
};