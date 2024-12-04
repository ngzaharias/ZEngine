#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor
{
	struct TableWindowRequestComponent;

	enum class EType
	{
		Guid,
		Number,
		String,
		Boolean,
	};

	struct Field
	{
		str::String m_Name = "<unknown>";
		EType m_Type = EType::String;
	};

	using Schema = Array<Field>;

	struct TableWindowComponent : public ecs::Component<TableWindowComponent>
	{
		int32 m_Identifier = 0;

		Map<str::Name, editor::Schema> m_TablesMap = {};
		Array<str::Name> m_TablesOpened = {};

		str::String m_WindowLabel = {};
		str::String m_BrowserLabel = {};
		str::String m_InspectorLabel = {};
	};

	class TableEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// components
			ecs::NameComponent,
			editor::TableWindowComponent,
			const editor::TableWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
