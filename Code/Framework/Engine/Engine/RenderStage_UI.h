#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct TextComponent;
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

namespace glfw
{
	class Window;
}

namespace eng
{
	class RenderStage_UI final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::settings::DebugSingleton,
			eng::TextComponent,
			eng::TransformComponent,
			eng::WindowManager>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexCoordBuffer = 0;
		uint32 m_TexDepthBuffer = 0;
		uint32 m_VertexBuffer = 0;
	};
}