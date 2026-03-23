#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace debug
{
	struct NetworkWindowComponent final : public ecs::Component
	{
		int32 m_Identifier = 0;
		str::String m_Message = {};
	};
}