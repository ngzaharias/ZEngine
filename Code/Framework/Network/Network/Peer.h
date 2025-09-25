#pragma once

#include "Core/Array.h"
#include "Core/Delegate.h"
#include "Core/String.h"
#include "Core/Types.h"
#include "Network/Messages.h"
#include "Network/PeerId.h"

class GameTime;

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

		void SendMessage(void* message);

		bool HasConnectionFailed() const { return false; };
		bool IsConnected() const { return false; };
		bool IsConnecting() const { return false; };
		bool IsRunning() const { return false; };

	protected:
		void ProcessMessage(const void* message);

		void OnClientConnected(const net::PeerId& peerId);
		void OnClientDisconnected(const net::PeerId& peerId);

	public:
		Delegate<void(const void* message)> m_OnProcessMessage;

	protected:
		net::Adaptor& m_Adaptor;
		net::Config& m_Config;

		DelegateCollection m_Collection = { };
	};
}

#include "Peer.inl"