#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace server::inventory
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
	struct StorageChangesSingleton;
	struct StorageComponent;
	struct StorageCreateRequestComponent;
	struct StorageCreateResultComponent;
	struct StorageDestroyRequestComponent;
	struct StorageDestroyResultComponent;
}

namespace server::inventory
{
	/// \brief Handles creating/destroying of storage entity and its components.
	class StorageSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			server::inventory::MemberAddResultComponent,
			server::inventory::MemberMoveResultComponent,
			server::inventory::MemberRemoveResultComponent,
			server::inventory::StorageChangesSingleton,
			server::inventory::StorageComponent,
			server::inventory::StorageCreateResultComponent,
			server::inventory::StorageDestroyResultComponent>
			::Read<
			server::inventory::MemberAddRequestComponent,
			server::inventory::MemberComponent,
			server::inventory::MemberMoveRequestComponent,
			server::inventory::MemberRemoveRequestComponent,
			server::inventory::OwnerComponent,
			server::inventory::StorageCreateRequestComponent,
			server::inventory::StorageDestroyRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberAddRequests(World& world);
		void ProcessMemberMoveRequests(World& world);
		void ProcessMemberRemoveRequests(World& world);
		void ProcessStorageRequests(World& world);
	};
}