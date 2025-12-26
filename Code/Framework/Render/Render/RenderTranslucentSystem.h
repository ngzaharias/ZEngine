#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct FlipbookComponent;
	struct SpriteComponent;
	struct TransformComponent;
	struct VisibilityComponent;
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

namespace render
{
	struct Batch;
	struct BatchId;
	struct Data;
	struct OpaqueComponent;
	struct TranslucentComponent;
}

namespace render
{
	class TranslucentSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager,
			render::TranslucentComponent>
			::Read<
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::FlipbookComponent,
			eng::settings::DebugSingleton,
			eng::SpriteComponent,
			eng::TransformComponent,
			eng::VisibilityComponent,
			eng::WindowManager,
			render::OpaqueComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void RenderBatch(World& world, const render::BatchId& id, const render::Batch& batch, const render::Data& data);

	private:
		uint32 m_ColourBuffer = 0;
		uint32 m_ModelBuffer = 0;
		uint32 m_TexParamBuffer = 0;
	};
}