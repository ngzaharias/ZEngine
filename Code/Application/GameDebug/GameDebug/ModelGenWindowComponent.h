#pragma once

#include "ECS/Component.h"

namespace debug
{
	struct ModelGenWindowComponent final : public ecs::Component 
	{ 
		float m_Radius = 1.f;
	};
};