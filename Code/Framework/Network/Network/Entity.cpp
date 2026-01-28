#include "Network/Entity.h"

#include "Core/MemBuffer.h"

bool net::Entity::IsUnassigned() const
{
	return m_Value == UINT64_MAX;
}

template<>
void MemBuffer::ReadCustom(net::Entity& value) const
{
	Read(value.m_Value);
}
template<>
void MemBuffer::WriteCustom(const net::Entity& value)
{
	Write(value.m_Value);
}
