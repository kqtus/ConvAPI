#pragma once
#include <string>

class ILevelFactory
{
public:
	virtual bool GetLevel(const std::string& level_name) = 0;
};