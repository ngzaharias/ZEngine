#pragma once

#include "Engine/ComponentRegistry.h"
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
		NetworkManager();

		void Update(const GameTime& gameTime);

		net::Adaptor& GetAdaptor() { return m_Adaptor; }
		const net::Adaptor& GetAdaptor() const { return m_Adaptor; }

		net::Host& GetHost() { return m_Host; }
		const net::Host& GetHost() const { return m_Host; }

		net::Peer& GetPeer() { return m_Peer; }
		const net::Peer& GetPeer() const { return m_Peer; }

		net::ComponentRegistry& GetComponentRegistry() { return m_ComponentRegistry; }
		const net::ComponentRegistry& GetComponentRegistry() const { return m_ComponentRegistry; }

	private:
		net::Adaptor m_Adaptor;
		net::Config m_Config;
		net::Host m_Host;
		net::Peer m_Peer;

		net::ComponentRegistry m_ComponentRegistry;
	};
}