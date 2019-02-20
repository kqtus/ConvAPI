#pragma once

namespace DirectX
{
	struct XMFLOAT4X4;
}

class ID3D11Device;
class ID3D11Buffer;

class ID3DXRenderData
{
public:
	virtual bool InitBuffers(ID3D11Device* d3d_device) = 0;
	virtual void ReleaseBuffers() = 0;

	virtual ID3D11Buffer* GetVertexBuffer() const = 0;
	virtual ID3D11Buffer* GetIndexBuffer() const = 0;

	virtual const DirectX::XMFLOAT4X4& GetWorldMat() const = 0;
};