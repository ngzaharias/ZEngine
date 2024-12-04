#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng
{
	struct StaticMeshAsset;

	struct StaticMeshComponent : public ecs::Component<StaticMeshComponent>
	{
		str::Guid m_StaticMesh = { };
	};

	struct StaticMeshAssetComponent : public ecs::Component<StaticMeshAssetComponent>
	{
		const eng::StaticMeshAsset* m_Asset = nullptr;
	};
}