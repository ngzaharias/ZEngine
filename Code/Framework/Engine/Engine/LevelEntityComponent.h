#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace eng::level
{
	/// \brief Attached to entities that were loaded from a level.
	struct EntityComponent final : public ecs::Component<EntityComponent>
	{
		EntityComponent() = default;
		EntityComponent(const str::Name & name) : m_Name(name) { }
		str::Name m_Name = {};
	};
}