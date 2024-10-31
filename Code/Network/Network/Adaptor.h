#pragma once

#include "Core/Delegate.h"
#include "Core/Types.h"

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
		Delegate<void(const net::PeerId&)> m_OnServerClientConnected;
		Delegate<void(const net::PeerId&)> m_OnServerClientDisconnected;
	};
}