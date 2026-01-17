#include "Network/MessageFactory.h"

#include "Network/Message.h"

net::Message* net::MessageFactory::Request(const net::EMessage type)
{
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	return entry.m_New(type);
}

void net::MessageFactory::Release(const net::Message* message)
{
	delete message;
}

void net::MessageFactory::Read(net::Message& data, const MemBuffer& buffer)
{
	const net::EMessage type = static_cast<net::EMessage>(data.m_Type);
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	entry.m_Read(data, buffer);
}

void net::MessageFactory::Write(const net::Message& data, MemBuffer& buffer)
{
	const net::EMessage type = static_cast<net::EMessage>(data.m_Type);
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	entry.m_Write(data, buffer);
}