#pragma once

#include "ECS/Component.h"

namespace client::hidden
{
	/// \brief Added to a hidden object entity when it has been revealed.
	struct RevealComponent final : public ecs::Component<RevealComponent> { };
}