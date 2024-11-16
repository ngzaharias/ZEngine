#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"

namespace eng
{
	struct ShaderAsset;
	struct SpriteAsset;
	struct Texture2DAsset;
}

namespace hexmap
{
	// \brief Attached to the same entity as SpriteComponent and holds a reference to the currently loaded assets.
	struct AssetComponent : public ecs::Component<AssetComponent>
	{
		const eng::ShaderAsset* m_Shader = nullptr;
		const eng::SpriteAsset* m_Sprite = nullptr;
		const eng::Texture2DAsset* m_Texture2D = nullptr;
	};
}