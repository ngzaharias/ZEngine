#pragma once

#include "Core/Colour.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage.h"
#include "Math/Vector.h"

namespace editor::settings
{
	struct LocalSingleton;
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

namespace editor
{
	class RenderStage_Axes final : public eng::RenderStage
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::AssetManager>
			::Read<
			editor::settings::LocalSingleton,
			eng::camera::EditorComponent,
			eng::camera::ProjectionComponent,
			eng::TransformComponent,
			eng::WindowManager>;

		void Initialise(ecs::EntityWorld& entityWorld) override;
		void Shutdown(ecs::EntityWorld& entityWorld) override;

		void Render(ecs::EntityWorld& entityWorld) override;

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