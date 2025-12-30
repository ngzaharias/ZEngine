#pragma once

#include "Core/Path.h"
#include "ECS/Singleton.h"

namespace editor::flipbook
{
	struct SettingsSingleton final : public ecs::Singleton<SettingsSingleton>
	{
		str::Path m_Extract = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};
}