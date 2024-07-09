#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace dbg
{
	struct NetworkWindowComponent : public ecs::Component<NetworkWindowComponent>
	{
		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;

		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};
}