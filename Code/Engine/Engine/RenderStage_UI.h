#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace glfw
{
	class Window;
}

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng
{
	class AssetManager;
	struct TextComponent;
	struct TransformComponent;

	class RenderStage_UI final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			const eng::camera::ProjectionComponent,
			const eng::TextComponent,
			const eng::TransformComponent>;

		void Initialise(ecs::EntityWorld& world) override;
		void Shutdown(ecs::EntityWorld& world) override;

		void Render(ecs::EntityWorld& world) override;

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexCoordBuffer = 0;
		uint32 m_TexDepthBuffer = 0;
		uint32 m_VertexBuffer = 0;
	};
}