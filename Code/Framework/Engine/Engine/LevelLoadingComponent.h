#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace eng::level
{
	enum class ELoadingState
	{
		FadeOut,
		Unload,
		Load,
		FadeIn,
	};

	/// \brief There exists one of these components for every level that is loaded.
	struct LoadingComponent final : public ecs::Component<LoadingComponent>
	{
		str::Name m_Name = {};

		ELoadingState m_StateCurrent = ELoadingState::FadeOut;
		ELoadingState m_StatePrevious = ELoadingState::FadeOut;
		float m_FadeInTime = 1.f;
		float m_FadeInTimer = 0.f;
		float m_FadeOutTime = 1.f;
		float m_FadeOutTimer = 0.f;
		bool m_IsSplash = false;
	};
}