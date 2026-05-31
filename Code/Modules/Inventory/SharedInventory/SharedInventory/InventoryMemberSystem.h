#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace shared::inventory
{
	struct MemberComponent;
	struct StorageChangesComponent;
	struct StorageComponent;
}

namespace shared::inventory
{
	/// \brief Handles adding/removing of member component on an entity.
	class MemberSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			shared::inventory::MemberComponent>
			::Read<
			shared::inventory::StorageComponent,
			shared::inventory::StorageChangesComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessAddRequests(World& world);
		void ProcessMoveRequests(World& world);
		void ProcessRemoveRequests(World& world);
	};
}