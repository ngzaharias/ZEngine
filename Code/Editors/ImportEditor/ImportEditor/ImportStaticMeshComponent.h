#pragma once

#include "Core/Path.h"
#include "ECS/Component.h"
#include "Engine/StaticMeshAsset.h"

namespace editor::importer
{
	struct StaticMeshComponent final : public ecs::SoloComponent
	{
		str::Path m_Filepath = {};
		eng::StaticMeshAsset m_Asset = {};
	};
}
