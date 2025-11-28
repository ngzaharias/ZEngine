#pragma once

#include "Core/String.h"
#include "Core/Variant.h"
#include "ECS/Event.h"
#include "Network/Types.h"

namespace eng::network
{
	struct Shutdown { };

	struct Startup
	{ 
		net::EMode m_Mode = net::EMode::Standalone;
		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;
		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};

	using Request = Variant<Shutdown, Startup>;

	/// \brief 
	struct ChangeRequestEvent final : public ecs::Event<ChangeRequestEvent>
	{
		Request m_Request = { };
	};
}