
template <typename TMessage>
void net::MessageFactory::Register(uint32 type)
{
	net::MessageEntry& entry = m_EntryMap[type];
	entry.m_New = &NewMethod<TMessage>;
	entry.m_Read = &ReadMethod<TMessage>;
	entry.m_Write = &WriteMethod<TMessage>;
}

template<typename TMessage>
net::Message* net::MessageFactory::NewMethod(const uint32 type)
{
	TMessage* message = new TMessage();
	message->m_Type = type;
	return message;
}

template<typename TMessage>
void net::MessageFactory::ReadMethod(net::Message& data, const MemBuffer& buffer)
{
	TMessage& message = static_cast<TMessage&>(data);
	buffer.Read(message);
}

template<typename TMessage>
void net::MessageFactory::WriteMethod(const net::Message& data, MemBuffer& buffer)
{
	const TMessage& message = static_cast<const TMessage&>(data);
	buffer.Write(message);
}