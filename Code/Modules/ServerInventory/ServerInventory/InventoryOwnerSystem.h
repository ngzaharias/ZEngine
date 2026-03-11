#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace server::inventory
{
	struct OwnerComponent;
	struct StorageChangesSingleton;
	struct StorageComponent;
}

namespace server::inventory
{
	/// \brief Handles creating/destroying of storage entity and its components.
	class OwnerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			server::inventory::OwnerComponent>
			::Read<
			server::inventory::StorageComponent,
			server::inventory::StorageChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessStorageChanges(World& world);
	};
}