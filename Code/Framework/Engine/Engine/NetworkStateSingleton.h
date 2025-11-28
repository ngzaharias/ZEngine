#pragma once

#include "Core/String.h"
#include "ECS/Singleton.h"
#include "Network/Types.h"

namespace eng::network
{
	/// \brief 
	struct StateSingleton final : public ecs::Singleton<StateSingleton>
	{
		net::EMode m_Mode = net::EMode::Standalone;
		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;
		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};
}