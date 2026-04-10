#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng
{
	struct StaticMeshTemplate final : public ecs::TemplateComponent
	{
		str::Guid m_StaticMesh = { };
	};
}