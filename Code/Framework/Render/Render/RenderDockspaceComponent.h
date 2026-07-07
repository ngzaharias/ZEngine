#pragma once

#include "ECS/Component.h"
#include "Math/Vector.h"

namespace render
{
	struct DockspaceComponent final : public ecs::StaticComponent
	{
		uint32 m_Id = 0;
		Vector2f m_Pos = Vector2f::Zero;
		Vector2f m_Size = Vector2f::Zero;
	};
}
