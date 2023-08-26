#pragma once

#include <Core/Array.h>
#include <Core/Set.h>
#include <Core/String.h>
#include <Core/Types.h>

#include <Network/Messages.h>
#include <Network/PeerId.h>

#include <entt/signal/sigh.hpp>

class GameTime;

namespace yojimbo
{
	class Message;
	class Server;
}

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

		void SendMessage(const net::PeerId& peerId, yojimbo::Message* message);

		bool IsRunning() const { return m_Server && m_Server->IsRunning(); };
		const Set<net::PeerId>& GetConnectedPeers() const { return m_PeersConnected; }

	protected:
		void ProcessMessage(const net::PeerId& peerId, const yojimbo::Message* message);

		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	public:
		entt::sigh<void(const net::PeerId& peerId, const yojimbo::Message* message)> m_OnProcessMessage;

	protected:
		net::Adaptor& m_Adaptor;
		net::Config& m_Config;
		yojimbo::Server* m_Server = nullptr;

		Array<entt::connection> m_Connections = { };
		Set<net::PeerId> m_PeersConnected = { };
		Set<net::PeerId> m_PeersDisconnected = { };
		int32 m_MaxPeers = 64;
	};
}

#include "Host.inl"