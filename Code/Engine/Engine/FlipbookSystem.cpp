#include "EnginePCH.h"
#include "Engine/FlipbookSystem.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/ShaderAsset.h"
#include "Engine/Texture2DAsset.h"

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;
}

void eng::FlipbookSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::FlipbookComponent>>())
	{
		auto& component = world.WriteComponent<eng::FlipbookComponent>(entity);
		component.m_TimeStart = gameTime.m_TotalTime;

		if (component.m_Flipbook.IsValid())
		{
			assetManager.RequestAsset<eng::FlipbookAsset>(component.m_Flipbook);
			if (const auto* flipbook = assetManager.FetchAsset<eng::FlipbookAsset>(component.m_Flipbook))
			{
				if (flipbook->m_Shader.IsValid())
					assetManager.RequestAsset<eng::ShaderAsset>(flipbook->m_Shader);
				if (flipbook->m_Texture2D.IsValid())
					assetManager.RequestAsset<eng::Texture2DAsset>(flipbook->m_Texture2D);
			}
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::FlipbookComponent>>())
	{
		auto& flipbookComponent = world.WriteComponent<eng::FlipbookComponent>(entity);
		if (!flipbookComponent.m_Flipbook.IsValid())
			continue;

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

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<const eng::FlipbookComponent>>())
	{
		const auto& component = world.ReadComponent<eng::FlipbookComponent>(entity, false);
		if (!component.m_Flipbook.IsValid())
			continue;

		if (const auto* flipbook = assetManager.FetchAsset<eng::FlipbookAsset>(component.m_Flipbook))
		{
			if (flipbook->m_Shader.IsValid())
				assetManager.ReleaseAsset<eng::ShaderAsset>(flipbook->m_Shader);
			if (flipbook->m_Texture2D.IsValid())
				assetManager.ReleaseAsset<eng::Texture2DAsset>(flipbook->m_Texture2D);
		}
		assetManager.ReleaseAsset<eng::FlipbookAsset>(component.m_Flipbook);
	}
}
