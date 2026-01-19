#include "GameDebugPCH.h"
#include "GameDebug/NetworkEvent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(debug::NetworkEvent& value) const
{
	Read(value.m_Data);
}

template<>
void MemBuffer::WriteCustom(const debug::NetworkEvent& value)
{
	Write(value.m_Data);
}