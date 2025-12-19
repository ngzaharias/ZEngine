#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/TrajectoryAsset.h"

namespace editor
{
	struct TrajectoryWindowComponent final : public ecs::Component<TrajectoryWindowComponent> 
	{ 
		int32 m_Identifier = 0;
		eng::TrajectoryAsset m_Asset = {};

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PlottingLabel = {};
	};
};