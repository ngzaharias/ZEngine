#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenVFXSystem.h"

#include "ClientHidden/HiddenRevealComponent.h"
#include "ClientHidden/HiddenVFXComponent.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelEntityComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/TransformComponent.h"

namespace
{
	const str::Guid strFlipbook = GUID("2df41b8ae3d947874c9d99cb2dbccf0a");
}

void client::hidden::VFXSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	return;

	using Query = ecs::query
		::Added<
		const hidden::RevealComponent>
		::Include<
		const ecs::NameComponent,
		const eng::level::EntityComponent,
		const eng::TransformComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const auto& objectName = view.ReadRequired<ecs::NameComponent>();
		const auto& objectLevel = view.ReadRequired<eng::level::EntityComponent>();
		const auto& objectTransform = view.ReadRequired<eng::TransformComponent>();

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

	for (auto&& view : world.Query<ecs::query::Include<const eng::FlipbookComponent, const hidden::VFXComponent>>())
	{
		const auto& flipbook = view.ReadRequired<eng::FlipbookComponent>();
		if (gameTime.m_TotalTime - flipbook.m_TimeStart > 10.f)
			world.DestroyEntity(view);
	}
}