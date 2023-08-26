#pragma once

#include <Core/Types.h>

#include <entt/signal/sigh.hpp>

#include <yojimbo/yojimbo.h>

namespace yojimbo
{
	class Allocator;
	class MessageFactory;
}

namespace net
{
	struct PeerId;

	class Adaptor : public yojimbo::Adapter
	{
	public:
		yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator) override;

		void OnServerClientConnected(int32 clientIndex) override;
		void OnServerClientDisconnected(int32 clientIndex) override;

	public:
		entt::sigh<void(const net::PeerId&)> m_OnServerClientConnected;
		entt::sigh<void(const net::PeerId&)> m_OnServerClientDisconnected;
	};
}