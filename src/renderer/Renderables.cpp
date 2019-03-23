#include "Renderables.h"
#include <d3d11.h>
#include <algorithm>

CRwModel::CRwModel()
{
	DirectX::XMStoreFloat4x4(&m_WorldMat, DirectX::XMMatrixIdentity());
}

CRwModel::CRwModel(rw::core::clump& copy)
	: rw::core::clump(copy)
{
	DirectX::XMStoreFloat4x4(&m_WorldMat, DirectX::XMMatrixIdentity());
}

CRwModel::~CRwModel()
{
}

bool CRwModel::InitBuffers(ID3D11Device* d3d_device)
{
	if (!d3d_device)
		return false;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(IRenderable::TVertex) * GetVertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinit_data;
	vinit_data.pSysMem = GetVertices();

	HRESULT res = d3d_device->CreateBuffer(&vbd, &vinit_data, &m_VertexBuffer);

	if (FAILED(res))
		return false;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * GetIndexCount();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinit_data;
	iinit_data.pSysMem = GetIndices();

	res = d3d_device->CreateBuffer(&ibd, &iinit_data, &m_IndexBuffer);

	if (FAILED(res))
		return false;

	return true;
}

void CRwModel::ReleaseBuffers()
{
	if (m_VertexBuffer)
		m_VertexBuffer->Release();

	if (m_IndexBuffer)
		m_IndexBuffer->Release();
}

ID3D11Buffer* CRwModel::GetVertexBuffer() const
{
	return m_VertexBuffer;
}

ID3D11Buffer* CRwModel::GetIndexBuffer() const
{
	return m_IndexBuffer;
}

const DirectX::XMFLOAT4X4& CRwModel::GetWorldMat() const
{
	return m_WorldMat;
}

int CRwModel::GetVertexCount() const
{
	return this->geometries[0]->data.header.vertex_count;
}

CRwModel::TVertices CRwModel::GetVertices() const
{
	TVertices verts = new TVertex[GetVertexCount()];
	for (int i = 0; i < GetVertexCount(); i++)
	{
		verts[i].pos = this->geometries[0]->data.vertices[i];

		std::swap(verts[i].pos.y, verts[i].pos.z);

		if (this->geometries[0]->data.header.flags & rw::rpGEOMETRYPRELIT)
		{
			auto col = this->geometries[0]->data.colours[i];
			verts[i].col.x = col.red / 255.f;
			verts[i].col.y = col.green / 255.f;
			verts[i].col.z = col.blue / 255.f;
			verts[i].col.w = col.alpha / 255.f;
		}
		else
		{
			verts[i].col.x = verts[i].col.y = verts[i].col.z = 0.5;
			verts[i].col.w = 1.0f;
		}
	}

	return verts;
}

int CRwModel::GetIndexCount() const
{
	return this->geometries[0]->data.header.face_count * 3;
}

CRwModel::TIndices CRwModel::GetIndices() const
{
	TIndices indices = new TIndex[GetIndexCount()];
	size_t offset = 0;

	auto add_face = [&](auto& arr, size_t& offset, const rw::core::geometry_data::face& face)
	{
		arr[offset + 0] = face.v2;
		arr[offset + 1] = face.v1;
		arr[offset + 2] = face.v3;
		offset += 3;
	};

	for (int i = 0; i < GetIndexCount() / 3; i++)
	{
		add_face(indices, offset, this->geometries[0]->data.faces[i]);
	}

	return indices;
}

vec3<float> CRwModel::GetPos() const
{
	return m_Pos;
}

void CRwModel::SetPos(const vec3<float>& pos)
{
	m_Pos = pos;
	OnWorldMatChanged();
}

vec3<float> CRwModel::GetRot() const
{
	return m_Rot;
}

void CRwModel::SetRot(const vec3<float>& rot)
{
	m_Rot = rot;
	OnWorldMatChanged();
}

vec3<float> CRwModel::GetScale() const
{
	return m_Scale;
}

void CRwModel::SetScale(const vec3<float>& scale)
{
	m_Scale = scale;
	OnWorldMatChanged();
}


void CRwModel::OnWorldMatChanged()
{
	DirectX::XMStoreFloat4x4(&m_WorldMat, DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z));
}