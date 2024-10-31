#pragma once

#include "Core/String.h"
#include "Core/StringView.h"
#include "ECS/Component.h"

namespace ecs
{
	struct NameComponent : public ecs::Component<NameComponent>
	{
		NameComponent() = default;
		NameComponent(const str::StringView& value) : m_Name(value) { }
		str::String m_Name = "<unknown>";
	};
}