#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng
{
	struct FrameBufferComponent final : public ecs::SingletonComponent<FrameBufferComponent>
	{
		uint32 m_ShadowBuffer = 0;
		uint32 m_ShadowTexture = 0;
		Vector2u m_ShadowSize = Vector2u::Zero;
	};
}