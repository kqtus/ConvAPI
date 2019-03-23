#pragma once

class ILevelResourcesDAO
{
public:
	virtual bool Disconnect(const std::wstring& archive_path) = 0;
	virtual void Disconnect() = 0;
};