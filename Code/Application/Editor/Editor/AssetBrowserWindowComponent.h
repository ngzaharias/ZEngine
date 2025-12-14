#pragma once

#include "Core/Array.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace editor
{
	struct AssetBrowserWindowComponent final : public ecs::Component<AssetBrowserWindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		Array<eng::AssetFile> m_Sorted = {};
	};
}
