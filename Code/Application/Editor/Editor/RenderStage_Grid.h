#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace editor::settings
{
	struct LocalSingleton;
}

namespace eng
{
	class AssetManager;
	class WindowManager;
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

namespace editor
{
	class AssetManager;
	struct TransformComponent;

	class RenderStage_Grid final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>
			::Read<
			editor::settings::LocalSingleton,
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
		uint32 m_TexCoordBuffer = 0;
		uint32 m_IndexBuffer = 0;
	};
}