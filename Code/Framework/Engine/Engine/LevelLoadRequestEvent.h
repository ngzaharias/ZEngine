#pragma once

#include "Core/Name.h"
#include "ECS/Event.h"

namespace eng::level
{
	/// \brief Request to load a specific level.
	struct LoadRequestEvent final : public ecs::Event<LoadRequestEvent>
	{
		LoadRequestEvent() = default;
		LoadRequestEvent(const str::Name& name) : m_Name(name) { }
		str::Name m_Name = {};
		bool m_IsSplash = false;
	}; 
}