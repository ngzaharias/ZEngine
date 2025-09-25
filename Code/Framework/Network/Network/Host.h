#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Types.h"
#include "Network/Messages.h"
#include "Network/PeerId.h"

class GameTime;

namespace net
{
	class Adaptor;

	struct Config;
	struct UserId;

	class Host
	{
	public:
		Host(net::Adaptor& adaptor, net::Config& config);

		void Startup(const str::String& ipAddress, const int32 port, const float time);
		void Shutdown();

		void Update(const GameTime& gameTime);

		template<typename TMessage>
		TMessage* CreateMessage(const net::PeerId& peerId, const EMessage type);

		void SendMessage(const net::PeerId& peerId, void* message);

		bool IsRunning() const { return false; };
		const Set<net::PeerId>& GetConnectedPeers() const { return m_PeersConnected; }

	protected:
		void ProcessMessage(const net::PeerId& peerId, const void* message);

		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	public:
		Delegate<void(const net::PeerId& peerId, const void* message)> m_OnProcessMessage;

	protected:
		net::Adaptor& m_Adaptor;
		net::Config& m_Config;

		DelegateCollection m_Collection = { };
		Set<net::PeerId> m_PeersConnected = { };
		Set<net::PeerId> m_PeersDisconnected = { };
		int32 m_MaxPeers = 64;
	};
}

#include "Host.inl"