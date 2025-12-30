#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/Texture2DAsset.h"

namespace editor::texture
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		int32 m_Identifier = 0;
		eng::Texture2DAsset m_Asset = {};

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
	};
}
