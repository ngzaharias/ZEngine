#include "ECS/DebugEvent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(ecs::DebugEvent& value) const
{
	Read(value.m_Data);
}

template<>
void MemBuffer::WriteCustom(const ecs::DebugEvent& value)
{
	Write(value.m_Data);
}