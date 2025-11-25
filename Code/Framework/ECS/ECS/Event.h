#pragma once

#include "Core/Types.h"

namespace ecs
{
	/// \brief Used to signal events.
	/// Each event is added on the next frame and deleted the frame after that.
	template<typename TEvent>
	struct Event { };

	using EventId = int32;
	struct EventTag {};
}