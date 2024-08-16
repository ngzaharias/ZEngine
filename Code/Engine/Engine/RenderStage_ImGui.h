#pragma once

#include "Engine/RenderStage.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace sfml
{
	class Window;
}

namespace eng
{
	class AssetManager;

	class RenderStage_ImGui final : public eng::RenderStage
	{
	public:
		RenderStage_ImGui(eng::AssetManager& assetManager, sfml::Window& window);
		~RenderStage_ImGui() override;

		void Initialise(ecs::EntityWorld& world) override;
		void Shutdown(ecs::EntityWorld& world) override;

		void Render(ecs::EntityWorld& world) override;

	private:
		sfml::Window& m_Window;
	};
}