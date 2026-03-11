#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace debug::inventory
{
	struct WindowComponent;
	struct WindowEvent;
}

namespace server::inventory
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

namespace debug::inventory
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::inventory::WindowComponent,
			server::inventory::MemberAddRequestComponent,
			server::inventory::MemberMoveRequestComponent,
			server::inventory::StorageCreateRequestComponent,
			server::inventory::StorageDestroyRequestComponent>
			::Read<
			debug::inventory::WindowEvent,
			server::inventory::MemberAddResultComponent,
			server::inventory::MemberMoveResultComponent,
			server::inventory::StorageComponent,
			server::inventory::StorageCreateResultComponent,
			server::inventory::StorageDestroyResultComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};