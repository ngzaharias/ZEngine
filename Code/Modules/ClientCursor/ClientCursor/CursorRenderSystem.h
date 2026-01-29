#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace client::cursor
{
	struct RenderComponent;
}

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct TransformComponent;
}

namespace render
{
	struct CursorComponent;
	struct ImGuiComponent;
}

namespace client::cursor
{
	class RenderSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			render::CursorComponent>
			::Read<
			client::cursor::RenderComponent,
			eng::TransformComponent,
			eng::WindowManager,
			render::ImGuiComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}