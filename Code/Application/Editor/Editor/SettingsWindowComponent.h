#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Editor/SettingsLocalStaticComponent.h"

namespace editor::settings
{
	struct WindowComponent final : public ecs::Component
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		LocalStaticComponent m_Local = {};
	};
}