#pragma once
#include <string>

class ILevelResourcesDesc;
class ILevelSerializer;
class ILevelDeserializer;

class IPhysicalLevel
{
public:
	virtual bool Load(ILevelResourcesDesc* rsc_desc, ILevelDeserializer* deserializer = nullptr) = 0;
	virtual bool Save(ILevelSerializer* serializer = nullptr) = 0;
	virtual bool SaveAs(ILevelResourcesDesc* rsc_desc, ILevelSerializer* serializer = nullptr) = 0;

	virtual void SetSerializer(ILevelSerializer* serializer) = 0;
	virtual ILevelSerializer* GetSerializer() const = 0;

	virtual void SetDeserializer(ILevelDeserializer* deserializer) = 0;
	virtual ILevelDeserializer* GetDeserializer() const = 0;
};