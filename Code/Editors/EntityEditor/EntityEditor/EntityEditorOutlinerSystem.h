#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	class EntityWorld;
	struct NameComponent;
}

namespace editor::entity
{
	class Commands;
	struct OpenOutlinerEvent;
	struct OutlinerComponent;
	struct SelectComponent;
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

namespace editor::entity
{
	class OutlinerSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::entity::Commands,
			editor::entity::OutlinerComponent,
			editor::entity::SelectComponent,
			eng::level::EntityComponent,
			eng::SpriteComponent,
			eng::TemplateComponent,
			eng::TransformComponent,
			eng::VisibilityComponent>
			::Read<
			editor::entity::OpenOutlinerEvent,
			eng::CameraComponent,
			eng::FlipbookComponent,
			eng::light::AmbientComponent,
			eng::light::DirectionalComponent,
			eng::light::PointComponent,
			eng::StaticMeshComponent,
			gamestate::EditorComponent>;

		OutlinerSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
