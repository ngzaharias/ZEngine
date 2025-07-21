#pragma once

#include "Core/Optional.h"
#include "ECS/Component.h"
#include "Engine/WindowModeEnum.h"
#include "Math/Vector.h"

namespace gui::settings_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent> { };

	struct CloseRequest final : public ecs::EventComponent<CloseRequest> { };
	struct OpenRequest final : public ecs::EventComponent<OpenRequest> { };
	struct ValueRequest final : public ecs::EventComponent<ValueRequest> 
	{ 
		// audio
		Optional<int32> m_EffectVolume = {};
		Optional<int32> m_MasterVolume = {};
		Optional<int32> m_MusicVolume = {};
		// gameplay
		Optional<float> m_MoveSpeed = {};
		Optional<float> m_ZoomRate = {};
		Optional<float> m_ZoomSpeed = {};
		// window
		Optional<int32> m_Monitor = {};
		Optional<int32> m_RefreshRate = {};
		Optional<Vector2u> m_Resolution = {};
		Optional<eng::EWindowMode> m_WindowMode = {};
	};
}