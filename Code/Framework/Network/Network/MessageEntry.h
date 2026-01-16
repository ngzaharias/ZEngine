#pragma once

class MemBuffer;

namespace net
{
	struct Message;
}

namespace net
{
	struct MessageEntry
	{
		using New = net::Message*(const uint32);
		using Read = void(net::Message&, const MemBuffer&);
		using Write = void(const net::Message&, MemBuffer&);

		New* m_New = nullptr;
		Read* m_Read = nullptr;
		Write* m_Write = nullptr;
	};
}