#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	class AssetManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct FrameBufferSingleton;
	struct StaticMeshComponent;
	struct TransformComponent;
}

namespace eng::light
{
	struct DirectionalComponent;
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
	struct ShadowComponent;
	struct UIPreComponent;
}

namespace render
{
	class ShadowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			//eng::AssetManager,
			//eng::FrameBufferSingleton,
			render::ShadowComponent>
			::Read<
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::light::DirectionalComponent,
			eng::settings::DebugSingleton,
			eng::StaticMeshComponent,
			eng::TransformComponent,
			render::UIPreComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void RenderBatch(World& world, const render::BatchId& id, const render::Batch& batch, const render::Data& data);

	private:
		uint32 m_ModelBuffer = 0;
	};
}