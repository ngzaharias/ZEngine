#pragma once

#include "ECS/Component.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

namespace eng::settings
{
	/// \brief Shared camera settings for all local users that is saved to a config file.
	struct CameraComponent : public ecs::SingletonComponent<CameraComponent>
	{
		Rotator m_RotateSpeed = Rotator(0.2f);
		float m_TranslateSpeed = 1000.f;
		float m_ZoomAmount = 300.f;
		float m_ZoomSpeed = 0.1f;
	};
}