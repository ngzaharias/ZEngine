#pragma once

namespace net
{
	struct Message
	{
		virtual ~Message() = default;

		uint32 m_Type = 0;
	};
}