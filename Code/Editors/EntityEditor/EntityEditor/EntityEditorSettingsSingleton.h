#pragma once

#include "Core/Path.h"
#include "ECS/Singleton.h"

namespace editor::entity
{
	struct SettingsSingleton final : public ecs::Singleton<SettingsSingleton>
	{
		str::Path m_Open = {};
		str::Path m_Save = {};
	};
}