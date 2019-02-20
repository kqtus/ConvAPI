#pragma once
#include <tuple>
#include "utilities/maths.h"

class IRenderable
{
public:
	struct SVertex
	{
		vec3<float> pos;
		vec4<float> col;
	};

	using TVertex = SVertex;
	using TVertices = TVertex*;

	virtual int GetVertexCount() const = 0;
	virtual TVertices GetVertices() const = 0;

	using TIndex = unsigned int;
	using TIndices = TIndex*;

	virtual int GetIndexCount() const = 0;
	virtual TIndices GetIndices() const = 0;

	virtual vec3<float> GetPos() const = 0;
	virtual void SetPos(const vec3<float>& pos) = 0;

	virtual vec3<float> GetRot() const = 0;
	virtual void SetRot(const vec3<float>& pos) = 0;

	virtual vec3<float> GetScale() const = 0;
	virtual void SetScale(const vec3<float>& pos) = 0;
};
