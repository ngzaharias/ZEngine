#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "ECS/Component.h"

namespace editor::entity
{
	struct EntityCreated
	{
		str::Guid m_Guid = {};
		str::String m_Data = {};
	};

	struct EntityDestroyed
	{
		str::Guid m_Guid = {};
		str::String m_Data = {};
	};

	struct EntityUpdated
	{
		str::Guid m_Guid = {};
		str::String m_Data = {};
	};

	struct EntitySelected
	{
		str::Guid m_After = {};
		str::Guid m_Before = {};
	};

	using HistoryData = Variant<
		EntityCreated,
		EntityDestroyed,
		EntityUpdated,
		EntitySelected>;

	struct HistoryComponent final : public ecs::StaticComponent
	{
		Array<HistoryData> m_RedoStack = {};
		Array<HistoryData> m_UndoStack = {};
	};
}
