#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace debug::crafting
{
	struct WindowComponent final : public ecs::Component 
	{ 
		int32 m_Identifier = 0;
		str::String m_Label = {};

		Array<str::Guid> m_Ingredients = {};
	};
};