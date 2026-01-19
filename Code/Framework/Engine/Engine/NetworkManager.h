#pragma once

#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Factory.h"

class GameTime;

namespace eng
{
	class NetworkManager final
	{
	public:
		NetworkManager();

		void Initialise();

		void Update(const GameTime& gameTime);

		net::Factory& GetFactory() { return m_Factory; }
		const net::Factory& GetFactory() const { return m_Factory; }

		net::Host& GetHost() { return m_Host; }
		const net::Host& GetHost() const { return m_Host; }

		net::Peer& GetPeer() { return m_Peer; }
		const net::Peer& GetPeer() const { return m_Peer; }

	private:
		net::Factory m_Factory;
		net::Host m_Host;
		net::Peer m_Peer;
	};
}