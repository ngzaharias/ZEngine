#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace softbody
{
	struct Link
	{
		Vector3f m_Position = Vector3f::Zero;
		float m_Radius = 100.f;
	};

	struct ChainComponent : public ecs::Component<ChainComponent>
	{
		Array<Link> m_Links = { 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{}, 
			Link{} };
	};
}