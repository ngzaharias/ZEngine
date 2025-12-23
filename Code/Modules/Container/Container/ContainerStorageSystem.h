#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace container
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

namespace container
{
	/// \brief Handles creating/destroying of storage entity and its components.
	class StorageSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			container::MemberAddResultComponent,
			container::MemberMoveResultComponent,
			container::MemberRemoveResultComponent,
			container::StorageChangesSingleton,
			container::StorageComponent,
			container::StorageCreateResultComponent,
			container::StorageDestroyResultComponent>
			::Read<
			container::MemberAddRequestComponent,
			container::MemberComponent,
			container::MemberMoveRequestComponent,
			container::MemberRemoveRequestComponent,
			container::OwnerComponent,
			container::StorageCreateRequestComponent,
			container::StorageDestroyRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberAddRequests(World& world);
		void ProcessMemberMoveRequests(World& world);
		void ProcessMemberRemoveRequests(World& world);
		void ProcessStorageRequests(World& world);
	};
}