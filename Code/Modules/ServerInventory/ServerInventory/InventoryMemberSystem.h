#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace server::inventory
{
	struct MemberComponent;
	struct StorageChangesStaticComponent;
	struct StorageComponent;
}

namespace server::inventory
{
	/// \brief Handles adding/removing of member component on an entity.
	class MemberSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			server::inventory::MemberComponent>
			::Read<
			server::inventory::StorageComponent,
			server::inventory::StorageChangesStaticComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessAddRequests(World& world);
		void ProcessMoveRequests(World& world);
		void ProcessRemoveRequests(World& world);
	};
}