#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace glfw
{
	class Window;
}

namespace eng
{
	class AssetManager;

	class RenderStage_ImGui final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const glfw::Window>;

		void Initialise(ecs::EntityWorld& world) override;
		void Shutdown(ecs::EntityWorld& world) override;

		void Render(ecs::EntityWorld& world) override;
	};
}