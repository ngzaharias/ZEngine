#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/String.h"
#include "Core/Types.h"
#include "Network/Messages.h"
#include "Network/PeerId.h"

class GameTime;

namespace yojimbo
{
	class Client;
	class Message;
}

namespace net
{
	class Adaptor;

	struct Config;

	class Peer final
	{
	public:
		Peer(net::Adaptor& adaptor, net::Config& config);

		void Startup(const str::String& ipAddress, const int32 port, const float time);
		void Shutdown();

		void Connect(const str::String& ipAddress, const int32 port);
		void Disconnect();

		void Update(const GameTime& gameTime);

		template<typename TMessage>
		TMessage* CreateMessage(const net::EMessage type);

		void SendMessage(yojimbo::Message* message);

		bool HasConnectionFailed() const { return m_Client && m_Client->ConnectionFailed(); };
		bool IsConnected() const { return m_Client && m_Client->IsConnected(); };
		bool IsConnecting() const { return m_Client && m_Client->IsConnecting(); };
		bool IsRunning() const { return m_Client; };

	protected:
		void ProcessMessage(const yojimbo::Message* message);

		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	public:
		Delegate<void(const yojimbo::Message* message)> m_OnProcessMessage;

	protected:
		net::Adaptor& m_Adaptor;
		net::Config& m_Config;
		yojimbo::Client* m_Client = nullptr;

		DelegateCollection m_Collection = { };
	};
}

#include "Peer.inl"