#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "ECS/Component.h"

namespace editor::inspector
{
	struct HistoryCreate
	{
		str::Guid m_Guid = {};
		str::String m_Data = {};
	};

	struct HistoryDestroy
	{
		str::Guid m_Guid = {};
		str::String m_Data = {};
	};

	struct HistoryUpdate
	{
		str::Guid m_Guid = {};
		str::String m_Data = {};
	};

	using HistoryData = Variant<
		HistoryCreate,
		HistoryDestroy,
		HistoryUpdate>;

	struct HistoryComponent final : public ecs::StaticComponent
	{
		Array<HistoryData> m_RedoStack = {};
		Array<HistoryData> m_UndoStack = {};
	};
}
