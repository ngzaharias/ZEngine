
template<typename TMessage>
TMessage* net::Host::RequestMessage(const net::EMessage type)
{
	net::Message* message = m_MessageFactory.Request(type);
	return static_cast<TMessage*>(message);
}