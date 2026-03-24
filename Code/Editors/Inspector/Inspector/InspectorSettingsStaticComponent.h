#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"

namespace editor::inspector
{
	struct SettingsStaticComponent final : public ecs::StaticComponent
	{
		str::Path m_Open = {};
		str::Path m_Save = {};
	};
}