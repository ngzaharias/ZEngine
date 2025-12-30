#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::table
{
	struct WindowComponent;
	struct WindowRequest;
}

namespace editor::table
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::table::WindowComponent>
			::Read<
			editor::table::WindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
