#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "GameDebug/FPSCounter.h"

namespace eng
{
	class WindowManager;
	struct VersionComponent;
}

namespace debug
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Read<
			eng::VersionComponent,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);

	private:
		debug::FPSCounter m_FPSCounter;
	};
}