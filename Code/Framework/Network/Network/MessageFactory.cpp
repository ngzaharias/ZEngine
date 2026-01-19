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

void net::MessageFactory::Read(net::Message& message, const MemBuffer& data)
{
	const net::EMessage type = static_cast<net::EMessage>(message.m_Type);
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	entry.m_Read(message, data);
}

void net::MessageFactory::Write(const net::Message& message, MemBuffer& data)
{
	const net::EMessage type = static_cast<net::EMessage>(message.m_Type);
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	entry.m_Write(message, data);
}