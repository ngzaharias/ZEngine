
template <typename TMessage>
void net::MessageFactory::Register(const uint32 type)
{
	Register<TMessage>(static_cast<net::EMessage>(type));
}

template <typename TMessage>
void net::MessageFactory::Register(const net::EMessage type)
{
	net::MessageEntry& entry = m_EntryMap[type];
	entry.m_New = &NewMethod<TMessage>;
	entry.m_Read = &ReadMethod<TMessage>;
	entry.m_Write = &WriteMethod<TMessage>;
}

template<typename TMessage>
net::Message* net::MessageFactory::NewMethod(const net::EMessage type)
{
	TMessage* message = new TMessage();
	message->m_Type = type;
	return message;
}

template<typename TMessage>
void net::MessageFactory::ReadMethod(net::Message& message, const MemBuffer& data)
{
	data.Read(static_cast<TMessage&>(message));
}

template<typename TMessage>
void net::MessageFactory::WriteMethod(const net::Message& message, MemBuffer& data)
{
	data.Write(static_cast<const TMessage&>(message));
}