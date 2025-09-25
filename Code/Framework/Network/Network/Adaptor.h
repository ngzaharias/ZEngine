#pragma once

#include "Core/Delegate.h"
#include "Core/Types.h"

namespace net
{
	struct PeerId;

	class Adaptor
	{
	public:
		void* CreateMessageFactory();

		void OnServerClientConnected(int32 clientIndex);
		void OnServerClientDisconnected(int32 clientIndex);

	public:
		Delegate<void(const net::PeerId&)> m_OnServerClientConnected;
		Delegate<void(const net::PeerId&)> m_OnServerClientDisconnected;
	};
}