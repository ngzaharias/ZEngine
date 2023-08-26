#pragma once

#include <Engine/RenderStage.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class AssetManager;

	class RenderStage_Lines final : public eng::RenderStage
	{
	public:
		RenderStage_Lines(eng::AssetManager& assetManager);
		~RenderStage_Lines() override;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
	};
}