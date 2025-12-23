#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace container
{
	struct MemberComponent;
	struct StorageChangesSingleton;
	struct StorageComponent;
}

namespace container
{
	/// \brief Handles adding/removing of member component on an entity.
	class MemberSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			container::MemberComponent>
			::Read<
			container::StorageComponent,
			container::StorageChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessAddRequests(World& world);
		void ProcessMoveRequests(World& world);
		void ProcessRemoveRequests(World& world);
	};
}