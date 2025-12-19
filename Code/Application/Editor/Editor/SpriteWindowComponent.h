#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/SpriteAsset.h"

namespace editor
{
	struct SpriteWindowComponent final : public ecs::Component<SpriteWindowComponent>
	{
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};

		eng::SpriteAsset m_Asset = {};
		str::Guid m_ShaderLoaded = {};
		str::Guid m_TextureLoaded = {};
	};
}
