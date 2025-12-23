#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace container
{
	struct MemberAddRequestComponent;
	struct MemberAddResultComponent;
	struct MemberMoveRequestComponent;
	struct MemberMoveResultComponent;
	struct MemberRemoveRequestComponent;
	struct StorageComponent;
	struct StorageCreateRequestComponent;
	struct StorageCreateResultComponent;
	struct StorageDestroyRequestComponent;
	struct StorageDestroyResultComponent;
}

namespace debug
{
	struct ContainerWindowComponent;
	struct ContainerWindowRequestComponent;
}

namespace debug
{
	class ContainerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			container::MemberAddRequestComponent,
			container::MemberMoveRequestComponent,
			container::StorageCreateRequestComponent,
			container::StorageDestroyRequestComponent,
			debug::ContainerWindowComponent>
			::Read<
			container::MemberAddResultComponent,
			container::MemberMoveResultComponent,
			container::StorageComponent,
			container::StorageCreateResultComponent,
			container::StorageDestroyResultComponent,
			debug::ContainerWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};