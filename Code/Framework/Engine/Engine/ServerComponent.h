#pragma once

#include "ECS/Component.h"

namespace eng
{
	/// \brief Component that is only present on server but registered on both.
	struct ServerComponent final : public ecs::SoloComponent
	{
	};
}