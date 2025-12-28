#pragma once

#include "Core/Array.h"
#include "Core/Colour.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Math/Vector.h"

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

namespace render
{
	struct OpaqueComponent;
	struct ShadowComponent;
}

namespace editor
{
	class RenderAxesSystem final : public ecs::System
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
			eng::TransformComponent,
			eng::WindowManager,
			render::ShadowComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void UpdateBuffers(const bool showX, const bool showY, const bool showZ);

	private:
		uint32 m_AttributeObject = 0;
		uint32 m_VertexBuffer = 0;
		uint32 m_ColourBuffer = 0;

		Array<Vector3f> m_Vertices = {};
		Array<Colour> m_Colours = {};
	};
}