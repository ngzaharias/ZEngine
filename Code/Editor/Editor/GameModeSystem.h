#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "GameDebug/FPSCounter.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace editor
{
	class GameModeSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::WindowManager,
			// Components
			const eng::settings::DebugComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}