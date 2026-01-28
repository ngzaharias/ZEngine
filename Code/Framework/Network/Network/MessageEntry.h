#pragma once

class MemBuffer;

namespace net
{
	enum EMessage : uint32;
	struct Message;
}

namespace net
{
	struct MessageEntry
	{
		using New = net::Message*(const net::EMessage);
		using Read = void(net::Message&, const MemBuffer&);
		using Write = void(const net::Message&, MemBuffer&);

		New* m_New = nullptr;
		Read* m_Read = nullptr;
		Write* m_Write = nullptr;
	};
}