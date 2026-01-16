#include "Network/Messages.h"

template<>
void MemBuffer::ReadCustom(net::DebugCommandMessage& value) const
{
	Read(value.m_Data);
}

template<>
void MemBuffer::WriteCustom(const net::DebugCommandMessage& value)
{
	Write(value.m_Data);
}