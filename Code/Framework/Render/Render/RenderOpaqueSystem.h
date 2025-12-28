#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct FrameBufferSingleton;
	struct StaticMeshComponent;
	struct TransformComponent;
}

namespace eng::light
{
	struct AmbientComponent;
	struct DirectionalComponent;
	struct PointComponent;
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
	struct ShadowComponent;
}

namespace render
{
	class OpaqueSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			//eng::AssetManager,
			//eng::FrameBufferSingleton,
			render::OpaqueComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::light::AmbientComponent,
			eng::light::DirectionalComponent,
			eng::light::PointComponent,
			eng::settings::DebugSingleton,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			eng::WindowManager,
			render::ShadowComponent>;

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