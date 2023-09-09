#pragma once

#include <Core/Array.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace container
{
	struct MemberAddRequestComponent;
	struct MemberAddResultComponent;
	struct MemberChangesComponent;
	struct MemberComponent;
	struct MemberRemoveRequestComponent;
	struct MemberRemoveResultComponent;
	struct StorageComponent;

	/// \brief Handles creating/destroying of storage entity and its components.
	class MemberSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			container::MemberAddResultComponent,
			container::MemberChangesComponent,
			container::MemberComponent,
			container::MemberRemoveResultComponent,
			const container::MemberAddRequestComponent,
			const container::MemberRemoveRequestComponent,
			const container::StorageComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessRequests(World& world);
	};
}