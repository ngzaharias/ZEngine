#pragma once

#include <Core/Guid.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

namespace eng
{
	struct FlipbookComponent : public ecs::Component<FlipbookComponent>
	{
		str::Guid m_Flipbook = { };
		Vector2u m_Size = Vector2u::Zero;
		int32 m_Index = 0;
		float m_Time = 0.f;
		bool m_IsLooping = true;
		bool m_IsPlaying = true;
	};
}