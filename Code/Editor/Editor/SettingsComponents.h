#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"

namespace editor::settings
{
	struct Entity
	{
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct Sprite
	{
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct Texture
	{
		str::Path m_Import = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct LocalComponent final : public ecs::SingletonComponent<LocalComponent>
	{
		Entity m_Entity = {};
		Sprite m_Sprite = {};
		Texture m_Texture = {};
	};
}