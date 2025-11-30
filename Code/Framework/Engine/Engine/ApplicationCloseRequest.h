#pragma once

#include "ECS/Event.h"

namespace eng::application
{
	/// \brief Request to close the application.
	struct CloseRequest final : public ecs::Event<CloseRequest> { };
}