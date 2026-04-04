#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

#include "imgui/Identifier.h"

namespace ecs
{
	class EntityWorld;
}

namespace editor::entity
{
	struct EntityCreatedEvent;
	struct EntityDestroyedEvent;
	struct EntitySelectedEvent;
	struct EntityUpdatedEvent;
	struct HistoryComponent;
	struct HistoryRedoEvent;
	struct HistorySaveEvent;
	struct HistoryUndoEvent;
	struct SelectComponent;
}

namespace eng
{
	class TemplateManager;
	struct TemplateComponent;
	struct TemplateMapComponent;
}

namespace editor::entity
{
	class HistorySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::entity::HistoryComponent,
			editor::entity::SelectComponent,
			eng::TemplateManager>
			::Read<
			editor::entity::EntityCreatedEvent,
			editor::entity::EntityDestroyedEvent,
			editor::entity::EntitySelectedEvent,
			editor::entity::EntityUpdatedEvent,
			editor::entity::HistoryRedoEvent,
			editor::entity::HistorySaveEvent,
			editor::entity::HistoryUndoEvent,
			eng::TemplateComponent,
			eng::TemplateMapComponent>;

		HistorySystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
