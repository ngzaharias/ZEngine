#include "SharedCursor/CursorTransformComponent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::cursor::TransformComponent& value) const
{
	Read(value.m_Translate);
}

template<>
void MemBuffer::WriteCustom(const shared::cursor::TransformComponent& value)
{
	Write(value.m_Translate);
}