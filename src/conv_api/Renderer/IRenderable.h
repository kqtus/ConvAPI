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
};
