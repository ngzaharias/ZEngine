#include "HiddenPCH.h"
#include "Hidden/HiddenVFXSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/TransformComponent.h"
#include "Hidden/HiddenRevealComponent.h"
#include "Hidden/HiddenVFXComponent.h"

namespace
{
	const str::Guid strFlipbook = GUID("2df41b8ae3d947874c9d99cb2dbccf0a");
}

void hidden::VFXSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	return;

	for (const ecs::Entity& objectEntity : world.Query<ecs::query::Added<const hidden::RevealComponent>>())
	{
		const auto& objectName = world.ReadComponent<ecs::NameComponent>(objectEntity);
		const auto& objectLevel = world.ReadComponent<eng::level::EntityComponent>(objectEntity);
		const auto& objectTransform = world.ReadComponent<eng::TransformComponent>(objectEntity);

		const ecs::Entity vfxEntity = world.CreateEntity();
		world.AddComponent<hidden::VFXComponent>(vfxEntity);

		auto& name = world.AddComponent<ecs::NameComponent>(vfxEntity);
		name.m_Name = std::format("{} {}", objectName.m_Name, "VFX");

		auto& level = world.AddComponent<eng::level::EntityComponent>(vfxEntity);
		level.m_Name = objectLevel.m_Name;

		auto& transform = world.AddComponent<eng::TransformComponent>(vfxEntity);
		transform.m_Translate = objectTransform.m_Translate;
		transform.m_Translate.z = -100.f;

		auto& flipbook = world.AddComponent<eng::FlipbookComponent>(vfxEntity);
		flipbook.m_Flipbook = strFlipbook;
		flipbook.m_Size = Vector2u(32, 32);
		flipbook.m_IsLooping = false;
		flipbook.m_TimeStart = gameTime.m_TotalTime;
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::FlipbookComponent, const hidden::VFXComponent>>())
	{
		const auto& flipbook = world.ReadComponent<eng::FlipbookComponent>(entity);
		if (gameTime.m_TotalTime - flipbook.m_TimeStart > 10.f)
			world.DestroyEntity(entity);
	}
}