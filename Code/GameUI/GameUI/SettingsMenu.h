#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::settings
{
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
			eng::settings::LocalComponent,
			gui::settings::WindowComponent,
			const gui::settings::CloseRequestComponent,
			const gui::settings::OpenRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}