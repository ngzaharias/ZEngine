#pragma once

#include "Core/Guid.h"
#include "ECS/Entity.h"

namespace render
{
	struct BatchId
	{
		ecs::Entity m_Entity = { };

		float m_Depth = 0.f;
		str::Guid m_TextureId = {};
		str::Guid m_ShaderId = {};
		str::Guid m_StaticMeshId = {};
	};
}