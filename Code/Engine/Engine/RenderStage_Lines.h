#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng::camera
{
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace eng
{
	class AssetManager;
	struct LinesComponent;
	struct TransformComponent;

	class RenderStage_Lines final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::LinesComponent,
			const eng::camera::ProjectionComponent,
			const eng::settings::DebugComponent,
			const eng::TransformComponent>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
	};
}