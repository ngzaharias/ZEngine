#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct DebugComponent;
}

namespace render
{
	struct OpaqueComponent;
	struct ShadowComponent;
}

namespace shared::hexmap
{
	struct LayerComponent;
	struct RootComponent;
}

namespace shared::hexmap
{
	class RenderSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			render::OpaqueComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::settings::DebugComponent,
			eng::TransformComponent,
			eng::WindowManager,
			render::ShadowComponent,
			shared::hexmap::LayerComponent,
			shared::hexmap::RootComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}