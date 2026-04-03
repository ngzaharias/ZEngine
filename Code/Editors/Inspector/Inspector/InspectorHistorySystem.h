#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

#include "imgui/Identifier.h"

namespace ecs
{
	class EntityWorld;
}

namespace editor::inspector
{
	struct HistoryComponent;
	struct HistoryRedoEvent;
	struct HistorySaveEvent;
	struct HistoryUndoEvent;
}

namespace eng
{
	class TemplateManager;
	struct TemplateComponent;
	struct TemplateMapComponent;
}

namespace editor::inspector
{
	class HistorySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::inspector::HistoryComponent,
			eng::TemplateManager>
			::Read<
			editor::inspector::HistoryRedoEvent,
			editor::inspector::HistorySaveEvent,
			editor::inspector::HistoryUndoEvent,
			eng::TemplateComponent,
			eng::TemplateMapComponent>;

		HistorySystem(ecs::EntityWorld& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		ecs::EntityWorld& m_World;
		imgui::Identifier m_WindowIds = {};
	};
}
