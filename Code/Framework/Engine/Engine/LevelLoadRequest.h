#pragma once

#include "Core/Name.h"
#include "ECS/Event.h"

namespace eng::level
{
	/// \brief Request to load a specific level.
	struct LoadRequest final : public ecs::Event<LoadRequest>
	{
		LoadRequest() = default;
		LoadRequest(const str::Name& name) : m_Name(name) { }
		str::Name m_Name = {};
		bool m_IsSplash = false;
	}; 
}