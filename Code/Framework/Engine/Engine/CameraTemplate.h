#pragma once

#include "ECS/Component.h"
#include "Engine/CameraTypes.h"

namespace eng
{
	/// \brief
	struct CameraTemplate final : public ecs::TemplateComponent
	{
		eng::Projection m_Projection = eng::Perspective();
	};
}