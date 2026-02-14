#pragma once

#include "Core/Path.h"
#include "ECS/Singleton.h"
#include "Editor/GizmoSettings.h"

namespace editor::settings
{
	struct LocalSingleton final : public ecs::Singleton<LocalSingleton>
	{
		Gizmos m_Gizmos = {};
	};
}