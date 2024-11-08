#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng
{
	struct FlipbookAsset;
	struct ShaderAsset;
	struct Texture2DAsset;

	struct FlipbookComponent : public ecs::Component<FlipbookComponent>
	{
		str::Guid m_Flipbook = { };
		Vector2u m_Size = Vector2u::Zero;
		int32 m_Index = 0;
		float m_TimeStart = 0.f;
		bool m_IsLooping = true;
		bool m_IsPlaying = true;
	};

	// \brief Attached to the same entity as FlipbookComponent and holds a reference to the currently loaded assets.
	struct FlipbookAssetComponent : public ecs::Component<FlipbookAssetComponent>
	{
		const eng::ShaderAsset* m_Shader = nullptr;
		const eng::FlipbookAsset* m_Flipbook = nullptr;
		const eng::Texture2DAsset* m_Texture2D = nullptr;
	};
}