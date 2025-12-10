#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
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
	struct ContainerWindowRequestComponent;
}

namespace debug
{
	struct ContainerWindowComponent final : public ecs::Component<ContainerWindowComponent> 
	{ 
		int32 m_Identifier = 0;
		str::String m_Label = {};
		ecs::Entity m_Storage = { };
	};

	class ContainerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			container::MemberAddRequestComponent,
			container::MemberMoveRequestComponent,
			container::StorageCreateRequestComponent,
			container::StorageDestroyRequestComponent,
			debug::ContainerWindowComponent,
			const container::MemberAddResultComponent,
			const container::MemberMoveResultComponent,
			const container::StorageComponent,
			const container::StorageCreateResultComponent,
			const container::StorageDestroyResultComponent,
			const debug::ContainerWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};