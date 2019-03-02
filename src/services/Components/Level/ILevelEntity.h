#pragma once

// From 'Renderer' proj
class IRenderable
{

};

struct EEntityType
{
	enum TYPE
	{
		STATIC,
		DYNAMIC,
		DESTROYABLE,
		SKELETAL
	};
};

class ILevelEntity : public IRenderable
{
public:
	virtual EEntityType::TYPE GetType() = 0;
};

#include <vector>
using TLevelEntitiyPtrs = std::vector<ILevelEntity*>;
