#pragma once

#include "Core/EnumHelpers.h"
#include "Core/MemBuffer.h"
#include "Core/Types.h"
#include "Network/Entity.h"

#include <yojimbo/yojimbo.h>

namespace net
{
	enum class ECommand
	{
		None,

		Ping,
	};

	enum class EMessage
	{
		ServerCommand,

		CreateEntity,
		DestroyEntity,

		AddComponent,
		UpdateComponent,
		RemoveComponent,
	};

	struct ServerCommandMessage : public yojimbo::Message
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			int32 value = EnumToValue(m_Data);
			serialize_int(stream, value, -INT32_MAX, INT32_MAX);
			m_Data = ValueToEnum<ECommand>(value);
			return true;
		}

		ECommand m_Data = ECommand::None;

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct CreateEntityMessage : public yojimbo::Message
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			return true;
		}

		net::Entity m_Entity = { };

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct DestroyEntityMessage : public yojimbo::Message
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			return true;
		}

		net::Entity m_Entity = { };

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct AddComponentMessage : public yojimbo::Message
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			serialize_int(stream, m_ComponentId, -INT32_MAX, INT32_MAX);

			int32 size = m_Data.GetCount();
			serialize_int(stream, size, 0, INT32_MAX);
			m_Data.GetData().Resize(size);
			for (uint8& data : m_Data.GetData())
				serialize_bits(stream, data, 8);
			return true;
		}

		net::Entity m_Entity = { };
		int32 m_ComponentId = -1;
		MemBuffer m_Data;

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct UpdateComponentMessage : public yojimbo::Message
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			serialize_int(stream, m_ComponentId, -INT32_MAX, INT32_MAX);

			int32 size = m_Data.GetCount();
			serialize_int(stream, size, 0, INT32_MAX);
			m_Data.GetData().Resize(size);
			for (uint8& data : m_Data.GetData())
				serialize_bits(stream, data, 8);
			return true;
		}

		net::Entity m_Entity = { };
		int32 m_ComponentId = -1;
		MemBuffer m_Data;

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	struct RemoveComponentMessage : public yojimbo::Message
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			serialize_int(stream, m_ComponentId, -INT32_MAX, INT32_MAX);
			return true;
		}

		net::Entity m_Entity = { };
		int32 m_ComponentId = -1;

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	YOJIMBO_MESSAGE_FACTORY_START(EngineMessageFactory, EnumToCount<EMessage>());
		YOJIMBO_DECLARE_MESSAGE_TYPE(EnumToValue(EMessage::ServerCommand), ServerCommandMessage);
		YOJIMBO_DECLARE_MESSAGE_TYPE(EnumToValue(EMessage::CreateEntity), CreateEntityMessage);
		YOJIMBO_DECLARE_MESSAGE_TYPE(EnumToValue(EMessage::DestroyEntity), DestroyEntityMessage);
		YOJIMBO_DECLARE_MESSAGE_TYPE(EnumToValue(EMessage::AddComponent), AddComponentMessage);
		YOJIMBO_DECLARE_MESSAGE_TYPE(EnumToValue(EMessage::RemoveComponent), RemoveComponentMessage);
		YOJIMBO_DECLARE_MESSAGE_TYPE(EnumToValue(EMessage::UpdateComponent), UpdateComponentMessage);
	YOJIMBO_MESSAGE_FACTORY_FINISH();
}