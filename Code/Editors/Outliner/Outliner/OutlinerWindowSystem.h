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
	struct PrototypeComponent;
	struct SpriteComponent;
	struct StaticMeshComponent;
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
	struct SelectSingleton;
	struct WindowComponent;
}

namespace eng::level
{
	struct EntityComponent;
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
			editor::outliner::SelectSingleton,
			editor::outliner::WindowComponent,
			eng::level::EntityComponent,
			eng::PrototypeComponent,
			eng::SpriteComponent,
			eng::TransformComponent,
			eng::VisibilityComponent>
			::Read<
			eng::CameraComponent,
			eng::FlipbookComponent,
			eng::StaticMeshComponent,
			editor::outliner::OpenWindowEvent,
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
