#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"

namespace editor::flipbook
{
	struct SettingsComponent final : public ecs::StaticComponent
	{
		str::Path m_Extract = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};
}