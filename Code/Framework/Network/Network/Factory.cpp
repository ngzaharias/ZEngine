#include "Network/Factory.h"

#include "Network/Message.h"

net::Message* net::Factory::Request(const net::EMessage type)
{
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	return entry.m_New(type);
}

void net::Factory::Release(const net::Message* message)
{
	delete message;
}

void net::Factory::Read(net::Message& message, const MemBuffer& data)
{
	const net::EMessage type = static_cast<net::EMessage>(message.m_Type);
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	entry.m_Read(message, data);
}

void net::Factory::Write(const net::Message& message, MemBuffer& data)
{
	const net::EMessage type = static_cast<net::EMessage>(message.m_Type);
	const net::MessageEntry& entry = m_EntryMap.Get(type);
	entry.m_Write(message, data);
}