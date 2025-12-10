#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace debug
{
	struct NetworkWindowComponent final : public ecs::Component<NetworkWindowComponent>
	{
		int32 m_Identifier = 0;

		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;

		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};
}