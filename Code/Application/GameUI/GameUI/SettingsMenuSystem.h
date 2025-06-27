#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class WindowManager;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace gui::settings
{
	struct CloseRequestComponent;
	struct OpenRequestComponent;
	struct WindowComponent;
}

namespace gui::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::WindowManager,
			// Components
			ecs::NameComponent,
			eng::settings::LocalComponent,
			gui::settings::WindowComponent,
			const gui::settings::CloseRequestComponent,
			const gui::settings::OpenRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}