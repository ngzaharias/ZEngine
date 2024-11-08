#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace eng
{
	struct FontAsset;

	struct TextComponent : ecs::Component<TextComponent>
	{
		str::String m_Text = { };
		str::Guid m_Font = { };
	};

	struct TextAssetComponent : ecs::Component<TextAssetComponent>
	{
		const eng::FontAsset* m_Font = nullptr;
	};
}