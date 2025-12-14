#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace editor
{
	struct EntityWindowComponent final : public ecs::Component<EntityWindowComponent>
	{
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_InspectorLabel = {};
	};
}
