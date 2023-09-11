#pragma once

#include <Core/Array.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

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
	struct StorageChangesComponent;
	struct StorageComponent;
	struct StorageCreateRequestComponent;
	struct StorageCreateResultComponent;
	struct StorageDestroyRequestComponent;
	struct StorageDestroyResultComponent;

	/// \brief Handles creating/destroying of storage entity and its components.
	class StorageSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			container::MemberAddResultComponent,
			container::MemberMoveResultComponent,
			container::MemberRemoveResultComponent,
			container::StorageChangesComponent,
			container::StorageComponent,
			container::StorageCreateResultComponent,
			container::StorageDestroyResultComponent,
			const container::MemberAddRequestComponent,
			const container::MemberComponent,
			const container::MemberMoveRequestComponent,
			const container::MemberRemoveRequestComponent,
			const container::OwnerComponent,
			const container::StorageCreateRequestComponent,
			const container::StorageDestroyRequestComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberAddRequests(World& world);
		void ProcessMemberMoveRequests(World& world);
		void ProcessMemberRemoveRequests(World& world);
		void ProcessStorageRequests(World& world);
	};
}