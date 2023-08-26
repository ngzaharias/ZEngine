#pragma once

#include <Core/String.h>
#include <Core/StringView.h>

#include <ECS/Component.h>

namespace eng
{
	struct NameComponent : public ecs::Component<NameComponent>
	{
		NameComponent() : m_Name() { }
		NameComponent(const str::StringView& value) : m_Name(value) { }

		str::String m_Name = { };
	};
}