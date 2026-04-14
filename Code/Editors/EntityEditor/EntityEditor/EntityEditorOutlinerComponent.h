#pragma once

#include "Core/Array.h"
#include "Core/Path.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace editor::entity
{
	struct OutlinerComponent final : public ecs::Component
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		Array<str::Path> m_Templates = {};
	};
}
