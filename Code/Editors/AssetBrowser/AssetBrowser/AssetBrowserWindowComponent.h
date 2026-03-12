#pragma once

#include "Core/Array.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "Engine/AssetFile.h"

namespace editor::assets
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		Array<eng::AssetFile> m_Sorted = {};
	};
}
