#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	class InputManager;
	class WindowManager;
}

namespace render
{
	struct CursorComponent;
	struct ImGuiComponent;
}

namespace render
{
	class CursorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			render::CursorComponent>
			::Read<
			eng::InputManager,
			eng::WindowManager,
			render::ImGuiComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}