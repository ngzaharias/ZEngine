#pragma once

#include "Core/EnumHelpers.h"
#include "Core/MemBuffer.h"
#include "Core/Types.h"
#include "Network/Entity.h"

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

	struct ServerCommandMessage
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
	};

	struct CreateEntityMessage
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			return true;
		}

		net::Entity m_Entity = { };
	};

	struct DestroyEntityMessage
	{
		template <typename Stream>
		bool Serialize(Stream& stream)
		{
			serialize_uint32(stream, m_Entity.m_Value);
			return true;
		}

		net::Entity m_Entity = { };
	};

	struct AddComponentMessage
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
	};

	struct UpdateComponentMessage
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
	};

	struct RemoveComponentMessage
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
	};
}