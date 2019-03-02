#include "LevelBase.h"

void CLevelBase::SetSerializer(ILevelSerializer* serializer)
{
	m_LvlSerializer = serializer;
}

ILevelSerializer* CLevelBase::GetSerializer() const
{
	return m_LvlSerializer;
}

void CLevelBase::SetDeserializer(ILevelDeserializer* deserializer)
{
	m_LvlDeserializer = deserializer;
}

ILevelDeserializer* CLevelBase::GetDeserializer() const
{
	return m_LvlDeserializer;
}