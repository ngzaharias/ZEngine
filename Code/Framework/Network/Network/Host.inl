
template<typename TMessage>
TMessage* net::Host::RequestMessage(const uint32 type)
{
	const net::EMessage etype = static_cast<net::EMessage>(type);
	net::Message* message = m_MessageFactory.Request(etype);
	return static_cast<TMessage*>(message);
}

template<typename TMessage>
TMessage* net::Host::RequestMessage(const net::EMessage type)
{
	net::Message* message = m_MessageFactory.Request(type);
	return static_cast<TMessage*>(message);
}