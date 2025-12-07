#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

namespace container
{
	struct OwnerComponent;
	struct StorageChangesSingleton;
	struct StorageComponent;

	/// \brief Handles creating/destroying of storage entity and its components.
	class OwnerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			container::OwnerComponent,
			const container::StorageComponent,
			// Singletons
			const container::StorageChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessStorageChanges(World& world);
	};
}