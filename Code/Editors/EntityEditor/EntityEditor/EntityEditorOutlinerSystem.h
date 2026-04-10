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
	class CommandManager;
	struct OpenOutlinerEvent;
	struct OutlinerComponent;
	struct SelectComponent;
}

namespace eng
{
	struct CameraTemplate;
	struct FlipbookTemplate;
	struct SpriteTemplate;
	struct StaticMeshTemplate;
	struct TransformTemplate;
	struct UUIDComponent;
	struct VisibilityTemplate;
}

namespace eng::light
{
	struct AmbientTemplate;
	struct DirectionalTemplate;
	struct PointTemplate;
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
			editor::entity::CommandManager,
			editor::entity::OutlinerComponent,
			editor::entity::SelectComponent,
			eng::SpriteTemplate,
			eng::TransformTemplate,
			eng::UUIDComponent,
			eng::VisibilityTemplate>
			::Read<
			editor::entity::OpenOutlinerEvent,
			eng::CameraTemplate,
			eng::FlipbookTemplate,
			eng::light::AmbientTemplate,
			eng::light::DirectionalTemplate,
			eng::light::PointTemplate,
			eng::StaticMeshTemplate,
			gamestate::EditorComponent>;

		OutlinerSystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
