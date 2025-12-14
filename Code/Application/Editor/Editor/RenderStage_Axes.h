#pragma once

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
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
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			const eng::WindowManager,
			// Components
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent,
			const eng::TransformComponent,
			// Singletons
			const editor::settings::LocalSingleton>;

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