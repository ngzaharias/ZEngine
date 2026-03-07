#pragma once

#include "Core/Name.h"
#include "ECS/Event.h"

namespace eng::level
{
	/// \brief Request to load a specific level.
	struct LoadEvent final : public ecs::Event<LoadEvent>
	{
		LoadEvent() = default;
		LoadEvent(const str::Name& name) : m_Name(name) { }
		str::Name m_Name = {};
		bool m_IsSplash = false;
	}; 
}