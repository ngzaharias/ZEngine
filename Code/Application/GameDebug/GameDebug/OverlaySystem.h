#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "GameDebug/FPSCounter.h"

namespace eng
{
	class WindowManager;
	struct VersionSingleton;
}

namespace debug
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Read<
			eng::VersionSingleton,
			eng::WindowManager>;

		void Update(World& world, const GameTime& gameTime);

	private:
		debug::FPSCounter m_FPSCounter;
	};
}