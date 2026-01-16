
template<typename TMessage>
TMessage* net::Host::RequestMessage(const uint32 type)
{
	net::Message* message = m_MessageFactory.Request(type);
	return static_cast<TMessage*>(message);
}