#pragma once

#include "Core/Map.h"
#include "Core/MemBuffer.h"
#include "Network/MessageEntry.h"

namespace net
{
	struct Message;
}

namespace net
{
	class MessageFactory final
	{
	public:
		template <typename TMessage>
		void Register(uint32 type);

		net::Message* Request(const uint32 type);
		void Release(const net::Message* message);

		void Read(net::Message& data, const MemBuffer& buffer);

		void Write(const net::Message& data, MemBuffer& buffer);

	private:
		template<typename TMessage>
		static net::Message* NewMethod(const uint32 type);
		template<typename TMessage>
		static void ReadMethod(net::Message& data, const MemBuffer& buffer);
		template<typename TMessage>
		static void WriteMethod(const net::Message& data, MemBuffer& buffer);

	private:
		Map<uint32, net::MessageEntry> m_EntryMap = {};
	};
}

#include "MessageFactory.inl"