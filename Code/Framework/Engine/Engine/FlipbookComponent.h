#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace eng
{
	struct FlipbookComponent final : public ecs::Component<FlipbookComponent>
		, ecs::IsPrototype
	{
		str::Guid m_Flipbook = { };
		Vector2u m_Size = Vector2u::Zero;
		int32 m_Index = 0;
		float m_TimeStart = 0.f;
		bool m_IsLooping = true;
		bool m_IsPlaying = true;
	};
}