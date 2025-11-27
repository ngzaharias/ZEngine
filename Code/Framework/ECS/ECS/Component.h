#pragma once

#include "Core/Types.h"

namespace ecs
{
	class EntityWorld;

	/// \brief The basic type of component and most commonly used.
	template<typename TComponent>
	struct Component { };

	using ComponentId = int32;
	struct ComponentTag {};
}