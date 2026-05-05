#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace inventory
{
	struct MemberComponent;
	struct StorageChangesComponent;
	struct StorageComponent;
}

namespace inventory
{
	/// \brief Handles adding/removing of member component on an entity.
	class MemberSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			inventory::MemberComponent>
			::Read<
			inventory::StorageComponent,
			inventory::StorageChangesComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessAddRequests(World& world);
		void ProcessMoveRequests(World& world);
		void ProcessRemoveRequests(World& world);
	};
}