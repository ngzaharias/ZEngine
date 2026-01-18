#pragma once

#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/MessageFactory.h"

class GameTime;

namespace net
{
	class NetworkManager final
	{
	public:
		NetworkManager();

		void Update(const GameTime& gameTime);

		net::Host& GetHost() { return m_Host; }
		const net::Host& GetHost() const { return m_Host; }

		net::Peer& GetPeer() { return m_Peer; }
		const net::Peer& GetPeer() const { return m_Peer; }

		net::MessageFactory& GetMessageFactory() { return m_MessageFactory; }
		const net::MessageFactory& GetMessageFactory() const { return m_MessageFactory; }

	private:
		net::Host m_Host;
		net::Peer m_Peer;

		net::MessageFactory m_MessageFactory;
	};
}