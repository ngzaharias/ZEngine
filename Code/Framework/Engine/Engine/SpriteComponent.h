#pragma once

#include "Core/Colour.h"
#include "Core/Guid.h"
#include "Core/Optional.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace eng
{
	struct ShaderAsset;
	struct SpriteAsset;
	struct Texture2DAsset;

	// \brief 
	struct SpriteComponent : public ecs::Component<SpriteComponent>
	{
		str::Guid m_Sprite = {};
		
		Optional<Colour> m_Colour = {};
		Optional<Vector2u> m_Size = {};
	};

	// \brief Attached to the same entity as SpriteComponent and holds a reference to the currently loaded assets.
	struct SpriteAssetComponent : public ecs::Component<SpriteAssetComponent>
	{
		const eng::ShaderAsset* m_Shader = nullptr;
		const eng::SpriteAsset* m_Sprite = nullptr;
		const eng::Texture2DAsset* m_Texture2D = nullptr;
	};
}