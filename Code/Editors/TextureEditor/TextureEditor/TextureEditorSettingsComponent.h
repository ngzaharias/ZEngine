#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"

namespace editor::texture
{
	struct SettingsComponent final : public ecs::StaticComponent
	{
		str::Path m_Import = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};
}