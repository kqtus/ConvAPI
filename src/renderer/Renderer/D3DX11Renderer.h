#pragma once
#include "Renderer.h"

#include <D3D11.h>
#include <DirectXMath.h>
#include "D3Dcompiler.h"

#include "../Common/d3dx11effect.h"

using namespace DirectX;

class CD3DX11Renderer : public CRenderer
{
public:
	CD3DX11Renderer(HWND target_window);
	virtual ~CD3DX11Renderer();

	/* IRenderer Interface */
	virtual bool Init() override;

	virtual void Update(float dt) override;
	virtual void Render() override;

	virtual void Move(float dx, float dy) override;
	virtual void Rotate(float dx, float dy) override;

	virtual void OnResize() override;

	virtual bool AddRenderSource(IRenderSource* render_src) override;
	virtual bool RemoveRenderSource(IRenderSource* render_src) override;

	/* End of IRenderer Interface */

	HWND GetWindowHandler();

protected:
	bool CreateDevice();

	void FillSwapChainDesc();
	bool CreateSwapChain();

	bool CreateRenderTargetView();
	bool CreateDepthStencil();

	void CreateViewport();

	virtual void BuildFX();
	virtual void BuildVertexLayout();

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

	ID3D11VertexShader* m_BgVertShader = nullptr;
	ID3D11PixelShader* m_BgPixelShader = nullptr;

	ID3D11Buffer* m_StaticObjsVB = nullptr;
	ID3D11Buffer* m_StaticObjsIB = nullptr;

	ID3DX11Effect* m_Fx = nullptr;
	ID3DX11EffectTechnique* m_Technique = nullptr;
	ID3DX11EffectMatrixVariable* m_FxWorldViewProj = nullptr;

	ID3D11InputLayout* m_InputLayout = nullptr;

	XMFLOAT4X4 m_Proj;
	XMFLOAT4X4 m_View;

	float m_Theta = 1.5f * 3.14f;
	float m_Phi = 0.25f * 3.14f;
	float m_Radius = 10.0f;
};