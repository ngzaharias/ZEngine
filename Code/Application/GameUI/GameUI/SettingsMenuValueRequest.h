#pragma once

#include "Core/Guid.h"
#include "Core/Optional.h"
#include "ECS/Event.h"
#include "Engine/WindowModeEnum.h"
#include "Math/Vector.h"

namespace gui::settings_menu
{
	struct ValueRequest final : public ecs::Event<ValueRequest> 
	{ 
		// audio
		Optional<float> m_EffectVolume = {};
		Optional<float> m_MasterVolume = {};
		Optional<float> m_MusicVolume = {};
		// camera
		Optional<float> m_MoveSpeed = {};
		Optional<float> m_ZoomRate = {};
		Optional<float> m_ZoomSpeed = {};
		Optional<str::Guid> m_Theme = {};
		// window
		Optional<int32> m_Monitor = {};
		Optional<int32> m_RefreshRate = {};
		Optional<Vector2u> m_Resolution = {};
		Optional<eng::EWindowMode> m_WindowMode = {};
	};
}