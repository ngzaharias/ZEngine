#pragma once

#include "Core/Guid.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace eng
{
	struct TemplateComponent final : public ecs::Component
	{
		TemplateComponent() = default;
		TemplateComponent(const str::Guid& value) : m_Guid(value) { }

		str::Guid m_Guid = {};
		str::Path m_Path = { };
	};
}