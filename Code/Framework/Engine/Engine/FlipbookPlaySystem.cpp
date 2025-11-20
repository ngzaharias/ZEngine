#include "EnginePCH.h"
#include "Engine/FlipbookPlaySystem.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FlipbookAsset.h"

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;
}

void eng::FlipbookPlaySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();

	using Query = ecs::query
		::Include<
		eng::FlipbookComponent>;
	for (const ecs::Entity& entity : world.Query<Query>())
	{
		auto& flipbookComponent = world.WriteComponent<eng::FlipbookComponent>(entity);
		if (!flipbookComponent.m_IsPlaying)
			continue;

		const auto* flipbookAsset = assetManager.FetchAsset<eng::FlipbookAsset>(flipbookComponent.m_Flipbook);
		if (!flipbookAsset || flipbookAsset->m_Frames.IsEmpty())
			continue;

		const int32 indexCount = flipbookAsset->m_Frames.GetCount();
		const float timeMax = indexCount / flipbookAsset->m_FPS;
		const float time = gameTime.m_TotalTime - flipbookComponent.m_TimeStart;

		if (indexCount >= 1)
		{
			const float index = math::Remap(time, 0.f, timeMax, 0.f, float(indexCount));
			flipbookComponent.m_Index = math::Round<int32>(index);

			if (flipbookComponent.m_IsLooping)
				flipbookComponent.m_Index %= indexCount;
		}
	}
}
