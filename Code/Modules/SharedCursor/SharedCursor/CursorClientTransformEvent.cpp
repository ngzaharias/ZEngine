#include "SharedCursor/CursorClientTransformEvent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::cursor::ClientTransformEvent& value) const
{
	Read(value.m_Translate);
}

template<>
void MemBuffer::WriteCustom(const shared::cursor::ClientTransformEvent& value)
{
	Write(value.m_Translate);
}