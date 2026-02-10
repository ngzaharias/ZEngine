#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/TrajectoryAsset.h"

namespace editor::theme
{
	struct WindowComponent final : public ecs::Component<WindowComponent> 
	{ 
		int32 m_Identifier = 0;
		str::String m_Label = {};
	};
};