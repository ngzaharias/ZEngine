#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/FlipbookAsset.h"

namespace editor
{
	struct FlipbookWindowComponent final : public ecs::Component<FlipbookWindowComponent>
	{
		int32 m_Identifier = 0;
		eng::FlipbookAsset m_Asset = {};

		str::String m_BatchingLabel = {};
		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};
	};
}
