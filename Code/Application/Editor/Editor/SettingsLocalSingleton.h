#pragma once

#include "Core/Path.h"
#include "ECS/Singleton.h"
#include "Editor/GizmoSettings.h"

namespace editor::settings
{
	struct LocalSingleton final : public ecs::Singleton
	{
		Gizmos m_Gizmos = {};
	};
}