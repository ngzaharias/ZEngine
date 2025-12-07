#pragma once

#include "Core/Array.h"
#include "ECS/Entity.h"
#include "ECS/System.h"

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
		using World = ecs::WorldView<
			container::MemberComponent,
			const container::StorageComponent,
			const container::StorageChangesSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessAddRequests(World& world);
		void ProcessMoveRequests(World& world);
		void ProcessRemoveRequests(World& world);
	};
}