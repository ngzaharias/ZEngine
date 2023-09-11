#pragma once

#include <Core/Array.h>
#include <Core/Guid.h>
#include <Core/Nullable.h>
#include <Core/Set.h>
#include <Core/SparseArray.h>
#include <Core/Variant.h>

#include <ECS/Component.h>
#include <ECS/Entity.h>

namespace container
{
	enum class EError
	{
		None,
		// member entity is dead
		MemberDead,
		// entity already a member of a storage
		MemberDuplicate,
		// target entity isn't a member
		MemberMissing,
		// member entity is unassigned
		MemberUnassigned,

		OwnerDead,
		OwnerDestroyed,
		OwnerInvalid,
		// owner entity is unassigned
		OwnerUnassigned,

		// storage entity is dead
		StorageDead,
		// owner already has a storage of that type
		StorageDuplicate,
		// target entity isn't a storage
		StorageMissing,
		// storage entity is unassigned
		StorageUnassigned,
	};

	enum class EMember : uint16
	{
		None = 0,
		// Reserved for user defined types
		Custom_Min = 100,
		Custom_Max = UINT16_MAX,
	};

	/// \brief 
	enum class EStorage : uint16
	{
		Main = 0,
		// Reserved for user defined types
		Custom_Min = 100,
		Custom_Max = UINT16_MAX,
	};

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

	//////////////////////////////////////////////////////////////////////////

	/// \brief Holds all information of a single member inside a container.
	struct MemberComponent final : public ecs::Component<MemberComponent>
	{
		ecs::Entity m_Storage = { };
		uint16 m_Count = 0;
		uint16 m_GridX = 0;
		uint16 m_GridY = 0;
		uint16 m_Type = static_cast<uint16>(EMember::None);
	};

	/// \brief Make a request for a member component.
	struct MemberAddRequestComponent final : public ecs::Component<MemberAddRequestComponent>
	{
		str::Guid m_TransactionId = { };
		// Storage that it is being added to.
		ecs::Entity m_Storage = { };

		// Member that is being added.
		ecs::Entity m_Member = { };
		uint16 m_Count = 1;
		uint16 m_GridX = 0;
		uint16 m_GridY = 0;
		uint16 m_Type = static_cast<uint16>(EMember::None);
	};

	/// \brief Result of a request made to a member component.
	struct MemberAddResultComponent final : public ecs::Component<MemberAddResultComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
		EError m_Error = { };
	};

	/// \brief Make a request for a member component.
	struct MemberMoveRequestComponent final : public ecs::Component<MemberMoveRequestComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
		ecs::Entity m_Member = { };
	};

	/// \brief Result of a request made to a member component.
	struct MemberMoveResultComponent final : public ecs::Component<MemberMoveResultComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
		EError m_Error = { };
	};

	/// \brief Make a request for a member component.
	struct MemberRemoveRequestComponent final : public ecs::Component<MemberRemoveRequestComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
	};

	/// \brief Result of a request made to a member component.
	struct MemberRemoveResultComponent final : public ecs::Component<MemberRemoveResultComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
		EError m_Error = { };
	};

	/// \brief 
	struct OwnerComponent final : public ecs::Component<OwnerComponent>
	{
		// All the storages the entity owns.
		SparseArray<int32, ecs::Entity> m_Storages = { };
	};

	/// \brief Holds a list of entities for a container.
	struct StorageComponent final : public ecs::Component<StorageComponent>
	{
		ecs::Entity m_Owner = { };
		int32 m_Limit = INT32_MAX;
		int32 m_Type = static_cast<int32>(EStorage::Main);

		// All the members of the storage.
		Set<ecs::Entity> m_Members = { };
	};

	/// \brief 
	struct StorageChangesComponent final : public ecs::SingletonComponent<StorageChangesComponent>
	{
		Array<MemberAdded> m_MemberAdded;
		Array<MemberMoved> m_MemberMoved;
		Array<MemberRemoved> m_MemberRemoved;

		Array<StorageChange> m_StorageCreated;
		Array<StorageChange> m_StorageDestroyed;
	};

	/// \brief Request that a new storage is created.
	struct StorageCreateRequestComponent final : public ecs::Component<StorageCreateRequestComponent>
	{
		str::Guid m_TransactionId = { };
		// The owner of the storage, can be unassigned.
		ecs::Entity m_Owner = { };
		// Maximum number of items allowed in the storage.
		int32 m_Limit = INT32_MAX;
		// The storage type.
		int32 m_Type = static_cast<int32>(EStorage::Main);
	};

	/// \brief Result of a create request made to a storage entity.
	struct StorageCreateResultComponent final : public ecs::Component<StorageCreateResultComponent>
	{
		str::Guid m_TransactionId = { };
		// The entity that was created, unassigned if it failed.
		ecs::Entity m_Storage = { };
		// Error result when creating the storage.
		EError m_Error = { };
	};

	/// \brief Request that the storage entity is destroyed.
	struct StorageDestroyRequestComponent final : public ecs::Component<StorageDestroyRequestComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
	};

	/// \brief Result of a destroy request made to a storage entity.
	struct StorageDestroyResultComponent final : public ecs::Component<StorageDestroyResultComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
		EError m_Error = { };
	};
}