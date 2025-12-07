#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Optional.h"
#include "Core/Set.h"
#include "Core/SparseArray.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/Singleton.h"

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
}