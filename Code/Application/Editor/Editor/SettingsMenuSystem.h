#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::settings
{
	struct LocalStaticComponent;
	struct WindowComponent;
	struct WindowEvent;
}

namespace editor::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView::Write<
			ecs::NameComponent,
			editor::settings::LocalStaticComponent,
			editor::settings::WindowComponent>
			::Read<
			editor::settings::WindowEvent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}