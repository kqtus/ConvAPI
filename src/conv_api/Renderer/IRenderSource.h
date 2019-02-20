#pragma once
#include <vector>

class IRenderable;

class IRenderSource
{
public:
	using TRenderables = std::vector<IRenderable*>;

	virtual void GetRenderables(TRenderables& renderables) const = 0;
	virtual std::string GetName() const = 0;
};
