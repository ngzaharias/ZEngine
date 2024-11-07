#pragma once

#include "Core/Guid.h"
#include "Core/Optional.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace eng
{
	struct SpriteAsset;

	// \brief 
	struct SpriteComponent : public ecs::Component<SpriteComponent>
	{
		str::Guid m_Sprite = {};
		Vector3f m_Colour = Vector3f::One;
		Vector2u m_Size = Vector2u::Zero;
	};

	// \brief Attached to the same entity as SpriteComponent and holds a reference to the currently loaded asset.
	struct SpriteAssetComponent : public ecs::Component<SpriteAssetComponent>
	{
		const eng::SpriteAsset* m_Asset = nullptr;
	};
}