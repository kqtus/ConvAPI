#include "Renderables.h"
#include <algorithm>

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