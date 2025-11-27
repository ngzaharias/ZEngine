#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"
#include "ECS/Singleton.h"
#include "Editor/GizmoSettings.h"

namespace editor::settings
{
	struct Entity
	{
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct Sprite
	{
		str::Path m_Extract = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct Texture
	{
		str::Path m_Import = {};
		str::Path m_Open = {};
		str::Path m_Save = {};
	};

	struct LocalSingleton final : public ecs::Singleton<LocalSingleton>
	{
		Entity m_Entity = {};
		Gizmos m_Gizmos = {};
		Sprite m_Sprite = {};
		Texture m_Texture = {};
	};
}