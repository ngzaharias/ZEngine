#pragma once

#include <Core/Name.h>
#include <Core/Variant.h>

#include <Network/Types.h>

namespace gamestate
{
	struct None { };

	struct LevelLoad
	{
		str::Name m_Name = { };
	};

	struct LevelUnload
	{ 
		str::Name m_Name = { };
	};

	struct NetworkHost
	{
		net::EMode m_Mode = net::EMode::Standalone;
		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;
		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};

	struct NetworkJoin
	{
		net::EMode m_Mode = net::EMode::Standalone;
		str::String m_ClientAddress = "0.0.0.0";
		int32 m_ClientPort = 30000;
		str::String m_ServerAddress = "127.0.0.1";
		int32 m_ServerPort = 40000;
	};

	struct NetworkStop
	{
	};

	using State = Variant<
		None,
		LevelLoad,
		LevelUnload,
		NetworkHost,
		NetworkJoin,
		NetworkStop>;
}