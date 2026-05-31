#pragma once

#include "ECS/Component.h"

namespace client::hidden
{
	/// \brief Counts the number of objects that are hidden and revealed.
	struct CountTemplate final : public ecs::TemplateComponent
	{
	};
}