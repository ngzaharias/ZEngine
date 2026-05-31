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

namespace shared::inventory
{
	struct MemberAddRequestComponent;
	struct MemberMoveRequestComponent;
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
			shared::inventory::MemberAddRequestComponent,
			shared::inventory::MemberMoveRequestComponent,
			shared::inventory::StorageCreateRequestComponent,
			shared::inventory::StorageDestroyRequestComponent>
			::Read<
			debug::inventory::WindowEvent,
			shared::inventory::StorageComponent,
			shared::inventory::StorageCreateResultComponent,
			shared::inventory::StorageDestroyResultComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
};