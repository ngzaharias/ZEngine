#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace editor
{
	struct TableWindowComponent;
	struct TableWindowRequest;
}

namespace editor
{
	class TableEditorSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// components
			ecs::NameComponent,
			editor::TableWindowComponent,
			const editor::TableWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
