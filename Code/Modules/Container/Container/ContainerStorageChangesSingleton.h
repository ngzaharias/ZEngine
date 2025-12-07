#pragma once

#include "Container/ContainerMemberEnum.h"
#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/Singleton.h"

namespace container
{
	struct MemberAdded
	{
		ecs::Entity m_Storage = {};
		ecs::Entity m_Member = {};
		uint16 m_Count = 1;
		uint16 m_GridX = 0;
		uint16 m_GridY = 0;
		uint16 m_Type = static_cast<uint16>(EMember::None);
	};

	struct MemberMoved
	{
		ecs::Entity m_Storage = {};
		ecs::Entity m_Member = {};
	};

	struct MemberRemoved
	{
		ecs::Entity m_Storage = {};
		ecs::Entity m_Member = {};
	};

	struct StorageChange
	{
		ecs::Entity m_Storage = {};
		ecs::Entity m_Owner = {};
		int32 m_Type = -1;
	};

	/// \brief 
	struct StorageChangesSingleton final : public ecs::Singleton<StorageChangesSingleton>
	{
		Array<MemberAdded> m_MemberAdded;
		Array<MemberMoved> m_MemberMoved;
		Array<MemberRemoved> m_MemberRemoved;

		Array<StorageChange> m_StorageCreated;
		Array<StorageChange> m_StorageDestroyed;
	};
}