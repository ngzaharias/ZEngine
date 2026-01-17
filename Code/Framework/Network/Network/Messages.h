#pragma once

#include "Core/EnumHelpers.h"
#include "Core/MemBuffer.h"
#include "Core/String.h"
#include "Core/TypeInfo.h"
#include "Core/Types.h"
#include "Network/Entity.h"
#include "Network/Message.h"

namespace net
{
	enum class ECommand
	{
		None,

		Ping,
	};

	enum class EMessage
	{
		Unassigned = 0,

		DebugCommand,
		ServerCommand,

		CreateEntity,
		DestroyEntity,

		AddComponent,
		UpdateComponent,
		RemoveComponent,
	};

	struct DebugCommandMessage : net::Message
	{
		str::String m_Data = {};
	};

	struct ServerCommandMessage : net::Message
	{
		ECommand m_Data = ECommand::None;
	};

	struct CreateEntityMessage : net::Message
	{
		net::Entity m_Entity = {};
	};

	struct DestroyEntityMessage : net::Message
	{
		net::Entity m_Entity = {};
	};

	struct AddComponentMessage : net::Message
	{
		TypeId m_TypeId = -1;

		net::Entity m_Entity = {};
		MemBuffer m_Data = {};
	};

	struct UpdateComponentMessage : net::Message
	{
		TypeId m_TypeId = -1;

		net::Entity m_Entity = {};
		MemBuffer m_Data = {};
	};

	struct RemoveComponentMessage : net::Message
	{
		TypeId m_TypeId = -1;
		net::Entity m_Entity = {};
	};
}