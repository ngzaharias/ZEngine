#pragma once

#include "Core/EnumHelpers.h"

#include <yojimbo/yojimbo.h>

namespace net
{
	constexpr const char* s_DefaultClientAddress = "0.0.0.0";
	constexpr const char* s_DefaultServerAddress = "127.0.0.1";
	constexpr int32 s_DefaultClientPort = 30000;
	constexpr int32 s_DefaultServerPort = 40000;
	constexpr uint8 s_DefaultPrivateKey[yojimbo::KeyBytes] = { 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0 };

	enum class EChannel 
	{
		Reliable,
		Unreliable,
	};

	struct Config : public yojimbo::ClientServerConfig
	{
		Config()
		{
			numChannels = EnumToCount<EChannel>();
			channel[(int)EChannel::Reliable].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
			channel[(int)EChannel::Unreliable].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
		}
	};
}