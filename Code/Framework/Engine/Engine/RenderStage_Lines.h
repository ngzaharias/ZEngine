#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct LinesSingleton;
	struct TransformComponent;
}

namespace eng::camera
{
	struct EditorComponent;
	struct ProjectionComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace eng
{
	class RenderStage_Lines final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::LinesSingleton>
			::Read<
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::settings::DebugSingleton,
			const eng::TransformComponent,
			const eng::WindowManager>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
	};
}