#pragma once

#include "Core/Types.h"

namespace ecs
{
	class EntityWorld;

	/// \brief The basic type of component and most commonly used.
	template<typename TComponent>
	struct Component { };

	/// \brief Component of which there is one that can exist at a time.
	/// Each component is added to the same entity that exists for the lifetime of the entity world.
	template<typename TComponent>
	struct SingletonComponent : public ecs::Component<TComponent> { };

	using ComponentId = int32;
	struct ComponentTag {};
}