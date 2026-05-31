#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::inventory
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

namespace shared::inventory
{
	/// \brief Handles creating/destroying of storage entity and its components.
	class StorageSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::inventory::MemberAddResultComponent,
			shared::inventory::MemberMoveResultComponent,
			shared::inventory::MemberRemoveResultComponent,
			shared::inventory::StorageChangesComponent,
			shared::inventory::StorageComponent,
			shared::inventory::StorageCreateResultComponent,
			shared::inventory::StorageDestroyResultComponent>
			::Read<
			shared::inventory::MemberAddRequestComponent,
			shared::inventory::MemberComponent,
			shared::inventory::MemberMoveRequestComponent,
			shared::inventory::MemberRemoveRequestComponent,
			shared::inventory::OwnerComponent,
			shared::inventory::StorageCreateRequestComponent,
			shared::inventory::StorageDestroyRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberAddRequests(World& world);
		void ProcessMemberMoveRequests(World& world);
		void ProcessMemberRemoveRequests(World& world);
		void ProcessStorageRequests(World& world);
	};
}