#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"
#include "Editor/GizmoSettings.h"

namespace editor::settings
{
	struct LocalStaticComponent final : public ecs::StaticComponent
	{
		Gizmos m_Gizmos = {};
	};
}