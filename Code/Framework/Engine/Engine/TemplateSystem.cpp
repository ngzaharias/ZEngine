#include "EnginePCH.h"
#include "Engine/TemplateSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateComponent.h"
#include "Engine/TemplateMapComponent.h"

void eng::TemplateSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Added<const eng::TemplateComponent>::Include<const eng::TemplateComponent>>())
	{
		const auto& templateComponent = view.ReadRequired<eng::TemplateComponent>();
		auto& mapComponent = world.WriteComponent<eng::TemplateMapComponent>();
		mapComponent.m_GuidToEntity.Set(templateComponent.m_Guid, view);
	}

	for (auto&& view : world.Query<ecs::query::Updated<const eng::TemplateComponent>::Include<const eng::TemplateComponent>>())
	{
		const auto& templateComponent = view.ReadRequired<eng::TemplateComponent>();
		auto& mapComponent = world.WriteComponent<eng::TemplateMapComponent>();
		mapComponent.m_GuidToEntity.Set(templateComponent.m_Guid, view);
	}
}