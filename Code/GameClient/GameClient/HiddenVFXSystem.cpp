#include "GameClientPCH.h"
#include "GameClient/HiddenVFXSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelComponents.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HiddenObjectComponents.h"

namespace
{
	const str::Guid strFlipbook = GUID("2df41b8a-e3d9-4787-4c9d-99cb2dbccf0a");
}

void hidden::VFXSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& objectEntity : world.Query<ecs::query::Added<const hidden::RevealedComponent>>())
	{
		const auto& objectLevel = world.GetComponent<const eng::level::EntityComponent>(objectEntity);
		const auto& objectTransform = world.GetComponent<const eng::TransformComponent>(objectEntity);

		const ecs::Entity vfxEntity = world.CreateEntity();
		auto& vfxLevel = world.AddComponent<eng::level::EntityComponent>(vfxEntity);
		vfxLevel.m_Name = objectLevel.m_Name;

		auto& vfxTransform = world.AddComponent<eng::TransformComponent>(vfxEntity);
		vfxTransform.m_Translate = objectTransform.m_Translate;
		vfxTransform.m_Translate.z = -100.f;

		auto& vfxFlipbook = world.AddComponent<eng::FlipbookComponent>(vfxEntity);
		vfxFlipbook.m_Flipbook = strFlipbook;
		vfxFlipbook.m_Size = Vector2u(32, 32);
		vfxFlipbook.m_IsLooping = false;
	}
}