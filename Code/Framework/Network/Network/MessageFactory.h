#pragma once

#include "Core/Map.h"
#include "Core/MemBuffer.h"
#include "Network/MessageEntry.h"

namespace net
{
	enum EMessage : uint32;
	struct Message;
}

namespace net
{
	class MessageFactory final
	{
	public:
		template <typename TMessage>
		void Register(const net::EMessage type);

		net::Message* Request(const net::EMessage type);
		void Release(const net::Message* message);

		void Read(net::Message& data, const MemBuffer& buffer);

		void Write(const net::Message& data, MemBuffer& buffer);

	private:
		template<typename TMessage>
		static net::Message* NewMethod(const net::EMessage type);
		template<typename TMessage>
		static void ReadMethod(net::Message& data, const MemBuffer& buffer);
		template<typename TMessage>
		static void WriteMethod(const net::Message& data, MemBuffer& buffer);

	private:
		Map<net::EMessage, net::MessageEntry> m_EntryMap = {};
	};
}

#include "MessageFactory.inl"