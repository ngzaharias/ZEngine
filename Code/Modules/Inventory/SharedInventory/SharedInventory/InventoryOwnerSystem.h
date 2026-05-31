#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::inventory
{
	struct OwnerComponent;
	struct StorageChangesComponent;
	struct StorageComponent;
}

namespace shared::inventory
{
	/// \brief Handles creating/destroying of storage entity and its components.
	class OwnerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::inventory::OwnerComponent>
			::Read<
			shared::inventory::StorageComponent,
			shared::inventory::StorageChangesComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessStorageChanges(World& world);
	};
}