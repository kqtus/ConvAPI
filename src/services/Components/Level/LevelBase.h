#pragma once
#include "ILevel.h"

class CLevelBase : public ILevel
{
public:
	virtual void SetSerializer(ILevelSerializer* serializer) override;
	virtual ILevelSerializer* GetSerializer() const override;

	virtual void SetDeserializer(ILevelDeserializer* deserializer) override;
	virtual ILevelDeserializer* GetDeserializer() const override;

private:
	ILevelSerializer* m_LvlSerializer = nullptr;
	ILevelDeserializer* m_LvlDeserializer = nullptr;
};