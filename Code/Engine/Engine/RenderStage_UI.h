#pragma once

#include <Engine/RenderStage.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace glfw
{
	class Window;
}

namespace eng
{
	class AssetManager;

	class RenderStage_UI final : public eng::RenderStage
	{
	public:
		RenderStage_UI(eng::AssetManager& assetManager);
		~RenderStage_UI() override;

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