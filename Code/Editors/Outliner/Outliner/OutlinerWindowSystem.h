#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct CameraComponent;
	struct FlipbookComponent;
	struct SpriteComponent;
	struct StaticMeshComponent;
	struct TemplateComponent;
	struct TransformComponent;
	struct VisibilityComponent;
}

namespace eng::light
{
	struct AmbientComponent;
	struct DirectionalComponent;
	struct PointComponent;
}

namespace editor::outliner
{
	struct OpenWindowEvent;
	struct SelectComponent;
	struct WindowComponent;
}

namespace eng::level
{
	struct EntityComponent;
}

namespace eng::light
{
	struct AmbientComponent;
	struct DirectionalComponent;
	struct PointComponent;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::outliner
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::outliner::SelectComponent,
			editor::outliner::WindowComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::TemplateComponent,
			eng::TransformComponent,
			eng::VisibilityComponent>
			::Read<
			eng::CameraComponent,
			eng::FlipbookComponent,
			eng::light::AmbientComponent,
			eng::light::DirectionalComponent,
			eng::light::PointComponent,
			eng::StaticMeshComponent,
			editor::outliner::OpenWindowEvent,
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
