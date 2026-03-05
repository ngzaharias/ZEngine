#pragma once

#include "ECS/Event.h"

namespace eng::application
{
	/// \brief Request to close the application.
	struct CloseRequestEvent final : public ecs::Event<CloseRequestEvent> { };
}