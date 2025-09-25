#pragma once

#include "Core/EnumHelpers.h"

namespace net
{
	constexpr const char* s_DefaultClientAddress = "0.0.0.0";
	constexpr const char* s_DefaultServerAddress = "127.0.0.1";
	constexpr int32 s_DefaultClientPort = 30000;
	constexpr int32 s_DefaultServerPort = 40000;
	constexpr uint8 s_DefaultPrivateKey[] = { 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0 };

	enum class EChannel 
	{
		Reliable,
		Unreliable,
	};

	struct Config
	{
	};
}