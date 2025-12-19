#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace editor
{
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

	struct TableWindowComponent final : public ecs::Component<TableWindowComponent>
	{
		int32 m_Identifier = 0;

		Map<str::Name, editor::Schema> m_TablesMap = {};
		Array<str::Name> m_TablesOpened = {};

		str::String m_WindowLabel = {};
		str::String m_BrowserLabel = {};
		str::String m_InspectorLabel = {};
	};
}
