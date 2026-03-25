#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"
#include "Editor/GizmoSettings.h"

namespace editor::settings
{
	struct LocalComponent final : public ecs::StaticComponent
	{
		Gizmos m_Gizmos = {};
	};
}