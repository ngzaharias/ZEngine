#pragma once

#include "Core/Array.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "GameClient/HexmapData.h"

namespace hexmap
{
	/// \brief 
	struct LayerComponent : ecs::Component<LayerComponent>
	{
		LayerPos m_Origin = {};
		ecs::Entity m_Root = {};

		Vector2i m_HexCount = {};
		Array<HexData> m_HexData = {};
	};
}