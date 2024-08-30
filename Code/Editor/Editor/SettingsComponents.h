#pragma once

#include "ECS/Component.h"

namespace edit::settings
{
	struct Texture
	{
		str::Path m_Import = {};
		str::Path m_Save = {};
	};

	struct LocalComponent final : public ecs::SingletonComponent<LocalComponent>
	{
		Texture m_Texture = {};
	};
}