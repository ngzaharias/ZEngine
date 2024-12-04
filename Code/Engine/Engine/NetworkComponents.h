#pragma once

#include "Core/Variant.h"
#include "ECS/Component.h"
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
	struct StateComponent : public ecs::SingletonComponent<StateComponent>
	{
		net::EMode m_Mode = net::EMode::Standalone;
		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;
		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};

	/// \brief 
	struct RequestComponent : public ecs::EventComponent<RequestComponent>
	{
		Request m_Request = { };
	};

	/// \brief 
	struct RequestFinishedComponent : public ecs::EventComponent<RequestFinishedComponent> { };
}