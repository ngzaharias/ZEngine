#pragma once

#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"

namespace ecs
{
	class EntityWorld;
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

namespace hexmap
{
	struct LayerComponent;
	struct RootComponent;

	class RenderStage final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			hexmap::LayerComponent,
			hexmap::RootComponent>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

	private:
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}