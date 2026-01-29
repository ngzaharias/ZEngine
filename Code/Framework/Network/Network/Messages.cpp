#include "Network/Messages.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(net::PeerHandshakeMessage& value) const
{
	Read(value.m_PeerId);
}
template<>
void MemBuffer::WriteCustom(const net::PeerHandshakeMessage& value)
{
	Write(value.m_PeerId);
}