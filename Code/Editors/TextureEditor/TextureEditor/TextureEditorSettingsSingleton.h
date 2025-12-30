#pragma once

#include "Core/Path.h"
#include "ECS/Singleton.h"

namespace editor::texture
{
	struct SettingsSingleton final : public ecs::Singleton<SettingsSingleton>
	{
		str::Path m_Import = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};
}