#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace gui::settings
{
	struct CloseRequestComponent final : public ecs::EventComponent<CloseRequestComponent> { };
	struct OpenRequestComponent final : public ecs::EventComponent<OpenRequestComponent> { };

	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		str::String m_Label = {};

		int32 m_EffectVolume = 50;
		int32 m_MusicVolume = 50;
	};
}