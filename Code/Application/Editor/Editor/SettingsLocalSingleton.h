#pragma once

#include "Core/Path.h"
#include "ECS/Singleton.h"
#include "Editor/GizmoSettings.h"

namespace editor::settings
{
	struct Entity
	{
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct LocalSingleton final : public ecs::Singleton<LocalSingleton>
	{
		Entity m_Entity = {};
		Gizmos m_Gizmos = {};
	};
}