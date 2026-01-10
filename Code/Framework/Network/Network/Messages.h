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
		ECommand m_Data = ECommand::None;
	};

	struct CreateEntityMessage
	{
		net::Entity m_Entity = {};
	};

	struct DestroyEntityMessage
	{
		net::Entity m_Entity = {};
	};

	struct AddComponentMessage
	{
		net::Entity m_Entity = {};
		int32 m_ComponentId = -1;
		MemBuffer m_Data;
	};

	struct UpdateComponentMessage
	{
		net::Entity m_Entity = {};
		int32 m_ComponentId = -1;
		MemBuffer m_Data = {};
	};

	struct RemoveComponentMessage
	{
		net::Entity m_Entity = {};
		int32 m_ComponentId = -1;
	};
}