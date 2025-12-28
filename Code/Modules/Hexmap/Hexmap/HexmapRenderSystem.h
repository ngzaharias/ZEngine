#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

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
}

namespace render
{
	struct OpaqueComponent;
	struct ShadowComponent;
}

namespace hexmap
{
	class RenderSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			//eng::AssetManager,
			render::OpaqueComponent>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			hexmap::LayerComponent,
			hexmap::RootComponent,
			render::ShadowComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}