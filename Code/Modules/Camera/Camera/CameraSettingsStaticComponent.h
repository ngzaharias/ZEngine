#pragma once

#include "ECS/Component.h"
#include "Math/Rotator.h"

namespace camera
{
	/// \brief Shared camera settings for all local users that is saved to a config file.
	struct SettingsStaticComponent final : public ecs::StaticComponent
	{
		Rotator m_RotateSpeed = Rotator(0.2f);
		float m_TranslateSpeed = 1000.f;
		float m_ZoomAmount = 300.f;
		float m_ZoomSpeed = 0.1f;
	};
}