#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace client::hidden
{
	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectTemplate final : public ecs::TemplateComponent
	{
		str::Guid m_Group = {};
	};
}