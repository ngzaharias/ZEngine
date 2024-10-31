#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng::settings
{
	struct DebugComponent;
	struct LocalComponent;
}

namespace gui::settings
{
	struct CloseRequestComponent;
	struct OpenRequestComponent;
	struct WindowComponent;

	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::settings::DebugComponent,
			eng::settings::LocalComponent,
			gui::settings::WindowComponent,
			const gui::settings::CloseRequestComponent,
			const gui::settings::OpenRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}