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
		void Register(const uint32 type);
		template <typename TMessage>
		void Register(const net::EMessage type);

		net::Message* Request(const net::EMessage type);
		void Release(const net::Message* message);

		void Read(net::Message& message, const MemBuffer& data);

		void Write(const net::Message& message, MemBuffer& data);

	private:
		template<typename TMessage>
		static net::Message* NewMethod(const net::EMessage type);
		template<typename TMessage>
		static void ReadMethod(net::Message& message, const MemBuffer& data);
		template<typename TMessage>
		static void WriteMethod(const net::Message& message, MemBuffer& data);

	private:
		Map<net::EMessage, net::MessageEntry> m_EntryMap = {};
	};
}

#include "MessageFactory.inl"