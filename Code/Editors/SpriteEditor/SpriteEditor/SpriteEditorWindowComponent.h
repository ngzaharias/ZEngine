#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/SpriteAsset.h"

namespace editor::sprite
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};

		eng::SpriteAsset m_Asset = {};
		str::Guid m_ShaderLoaded = {};
		str::Guid m_TextureLoaded = {};
		bool m_UnsavedChanges = false;

		Vector2f m_RegionOffset = Vector2f::Zero;
		Vector2f m_RegionZoom = Vector2f::Zero;
	};
}
