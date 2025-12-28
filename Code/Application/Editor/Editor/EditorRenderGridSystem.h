#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::settings
{
	struct LocalSingleton;
}

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
	struct DebugSingleton;
}

namespace render
{
	struct OpaqueComponent;
	struct ShadowComponent;
}

namespace editor
{
	class RenderGridSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			//eng::AssetManager,
			render::OpaqueComponent>
			::Read<
			editor::settings::LocalSingleton,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::settings::DebugSingleton,
			eng::TransformComponent,
			eng::WindowManager,
			render::ShadowComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
		uint32 m_TexCoordBuffer = 0;
		uint32 m_IndexBuffer = 0;
	};
}