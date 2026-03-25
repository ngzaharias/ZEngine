#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct LinesComponent;
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

namespace render
{
	class DebugSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			eng::LinesComponent,
			render::OpaqueComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::settings::DebugComponent,
			eng::TransformComponent,
			eng::WindowManager,
			render::ShadowComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
	};
}