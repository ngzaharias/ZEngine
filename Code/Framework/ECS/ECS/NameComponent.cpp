#include "ECS/NameComponent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(ecs::NameComponent& value) const
{
	Read(value.m_Name);
}
template<>
void MemBuffer::WriteCustom(const ecs::NameComponent& value)
{
	Write(value.m_Name);
}