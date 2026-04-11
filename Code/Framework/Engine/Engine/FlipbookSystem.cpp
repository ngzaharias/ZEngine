#include "EnginePCH.h"
#include "Engine/FlipbookSystem.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FlipbookTemplate.h"

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;
}

void eng::FlipbookSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();

	using UpdateQuery = ecs::query
		::Include<eng::FlipbookComponent>;
	for (auto&& view : world.Query<UpdateQuery>())
	{
		auto& flipbookComponent = view.WriteRequired<eng::FlipbookComponent>();
		if (!flipbookComponent.m_IsPlaying)
			continue;

		const auto* flipbookAsset = assetManager.ReadAsset<eng::FlipbookAsset>(flipbookComponent.m_Flipbook);
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

	using AddedQuery = ecs::query
		::Added<const eng::FlipbookTemplate>
		::Include<const eng::FlipbookTemplate>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& flipbookTemplate = view.ReadRequired<eng::FlipbookTemplate>();
		auto& flipbookComponent = world.AddComponent<eng::FlipbookComponent>(view);
		flipbookComponent.m_Flipbook = flipbookTemplate.m_Flipbook;
		flipbookComponent.m_Size = flipbookTemplate.m_Size;
		flipbookComponent.m_Index = flipbookTemplate.m_Index;
		flipbookComponent.m_TimeStart = flipbookTemplate.m_TimeStart;
		flipbookComponent.m_IsLooping = flipbookTemplate.m_IsLooping;
		flipbookComponent.m_IsPlaying = flipbookTemplate.m_IsPlaying;
	}

	using UpdatedQuery = ecs::query
		::Updated<const eng::FlipbookTemplate>
		::Include<eng::FlipbookComponent, const eng::FlipbookTemplate>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& flipbookTemplate = view.ReadRequired<eng::FlipbookTemplate>();
		auto& flipbookComponent = view.WriteRequired<eng::FlipbookComponent>();
		flipbookComponent.m_Flipbook = flipbookTemplate.m_Flipbook;
		flipbookComponent.m_Size = flipbookTemplate.m_Size;
		flipbookComponent.m_Index = flipbookTemplate.m_Index;
		flipbookComponent.m_TimeStart = flipbookTemplate.m_TimeStart;
		flipbookComponent.m_IsLooping = flipbookTemplate.m_IsLooping;
		flipbookComponent.m_IsPlaying = flipbookTemplate.m_IsPlaying;
	}

	for (auto&& view : world.Query<ecs::query::Removed<const eng::FlipbookTemplate>>())
	{
		world.RemoveComponent<eng::FlipbookComponent>(view);
	}
}
