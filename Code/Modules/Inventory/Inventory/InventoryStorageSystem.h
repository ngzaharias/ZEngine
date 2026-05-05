#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace inventory
{
	struct MemberAddRequestComponent;
	struct MemberAddResultComponent;
	struct MemberChangesComponent;
	struct MemberComponent;
	struct MemberMoveRequestComponent;
	struct MemberMoveResultComponent;
	struct MemberRemoveRequestComponent;
	struct MemberRemoveResultComponent;
	struct OwnerComponent;
	struct StorageChangesComponent;
	struct StorageComponent;
	struct StorageCreateRequestComponent;
	struct StorageCreateResultComponent;
	struct StorageDestroyRequestComponent;
	struct StorageDestroyResultComponent;
}

namespace inventory
{
	/// \brief Handles creating/destroying of storage entity and its components.
	class StorageSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			inventory::MemberAddResultComponent,
			inventory::MemberMoveResultComponent,
			inventory::MemberRemoveResultComponent,
			inventory::StorageChangesComponent,
			inventory::StorageComponent,
			inventory::StorageCreateResultComponent,
			inventory::StorageDestroyResultComponent>
			::Read<
			inventory::MemberAddRequestComponent,
			inventory::MemberComponent,
			inventory::MemberMoveRequestComponent,
			inventory::MemberRemoveRequestComponent,
			inventory::OwnerComponent,
			inventory::StorageCreateRequestComponent,
			inventory::StorageDestroyRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberAddRequests(World& world);
		void ProcessMemberMoveRequests(World& world);
		void ProcessMemberRemoveRequests(World& world);
		void ProcessStorageRequests(World& world);
	};
}