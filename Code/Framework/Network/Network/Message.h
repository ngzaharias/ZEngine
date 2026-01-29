#pragma once

#include "Core/Types.h"

namespace net
{
	struct Message
	{
		virtual ~Message() = default;

		// net::EMessage
		uint32 m_Type = 0;
	};
}