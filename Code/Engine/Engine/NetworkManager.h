#pragma once

#include "Engine/ComponentSerializer.h"
#include "Network/Adaptor.h"
#include "Network/Config.h"
#include "Network/Host.h"
#include "Network/Peer.h"

class GameTime;

namespace eng
{
	class NetworkManager final
	{
	public:
		NetworkManager(net::ComponentSerializer& serializer);

		void Update(const GameTime& gameTime);

		net::Adaptor& GetAdaptor() { return m_Adaptor; }
		const net::Adaptor& GetAdaptor() const { return m_Adaptor; }

		net::Host& GetHost() { return m_Host; }
		const net::Host& GetHost() const { return m_Host; }

		net::Peer& GetPeer() { return m_Peer; }
		const net::Peer& GetPeer() const { return m_Peer; }

		net::ComponentSerializer& GetSerializer() { return m_Serializer; }
		const net::ComponentSerializer& GetSerializer() const { return m_Serializer; }

	private:
		net::ComponentSerializer& m_Serializer;

		net::Adaptor m_Adaptor;
		net::Config m_Config;
		net::Host m_Host;
		net::Peer m_Peer;
	};
}