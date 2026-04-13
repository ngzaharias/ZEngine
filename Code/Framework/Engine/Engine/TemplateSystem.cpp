#include "EnginePCH.h"
#include "Engine/TemplateSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDMapComponent.h"

void eng::TemplateSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const eng::UUIDComponent>::Include<const eng::UUIDComponent>>())
	{
		const auto& uuidComponent = view.ReadRequired<eng::UUIDComponent>();
		auto& mapComponent = world.WriteComponent<eng::UUIDMapComponent>();
		mapComponent.m_ToEntity.Set(uuidComponent.m_UUID, view);
	}

	for (auto&& view : world.Query<ecs::query::Updated<const eng::UUIDComponent>::Include<const eng::UUIDComponent>>())
	{
		const auto& uuidComponent = view.ReadRequired<eng::UUIDComponent>();
		auto& mapComponent = world.WriteComponent<eng::UUIDMapComponent>();
		mapComponent.m_ToEntity.Set(uuidComponent.m_UUID, view);
	}
}