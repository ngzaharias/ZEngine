#pragma once

#include <Core/Array.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace container
{
	struct OwnerComponent;
	struct StorageChangesComponent;
	struct StorageComponent;

	/// \brief Handles creating/destroying of storage entity and its components.
	class OwnerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			container::OwnerComponent,
			const container::StorageChangesComponent,
			const container::StorageComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessStorageChanges(World& world);
	};
}