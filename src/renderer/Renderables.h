#include "../formats/rw/rwcore.h"
#include "Renderer/IRenderable.h"
#include "Renderer/ID3DXRenderData.h"

#include <DirectXMath.h>

class CRwModel 
	: public rw::core::clump 
	, public IRenderable
	, public ID3DXRenderData
{
public:
	CRwModel();
	virtual ~CRwModel();

	virtual bool InitBuffers(ID3D11Device* d3d_device) override;
	virtual void ReleaseBuffers() override;

	virtual ID3D11Buffer* GetVertexBuffer() const override;
	virtual ID3D11Buffer* GetIndexBuffer() const override;
	
	virtual const DirectX::XMFLOAT4X4& GetWorldMat() const override;

	virtual int GetVertexCount() const override;
	virtual TVertices GetVertices() const override;

	virtual int GetIndexCount() const override;
	virtual TIndices GetIndices() const override;

	virtual vec3<float> GetPos() const override;
	virtual void SetPos(const vec3<float>& pos) override;

	virtual vec3<float> GetRot() const override;
	virtual void SetRot(const vec3<float>& pos) override;

	virtual vec3<float> GetScale() const override;
	virtual void SetScale(const vec3<float>& pos) override;

protected:
	virtual void OnWorldMatChanged();

	vec3<float> m_Pos = { 0.f, 0.f, 0.f };
	vec3<float> m_Rot = { 0.f, 0.f, 0.f };
	vec3<float> m_Scale = { 1.f, 1.f, 1.f };

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;

	DirectX::XMFLOAT4X4 m_WorldMat;
};