#pragma once

#include <Core/Array.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace container
{
	struct MemberChangesComponent;
	struct MemberComponent;
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
			container::StorageChangesComponent,
			container::StorageComponent,
			container::StorageCreateResultComponent,
			container::StorageDestroyResultComponent,
			const container::MemberChangesComponent,
			const container::MemberComponent,
			const container::OwnerComponent,
			const container::StorageCreateRequestComponent,
			const container::StorageDestroyRequestComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberChanges(World& world);
		void ProcessStorageRequests(World& world);
	};
}