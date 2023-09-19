#include "GameClientPCH.h"
#include "GameClient/TransformSystem.h"

#include <Core/GameTime.h>
#include <Core/Quaternion.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/TransformComponent.h>

#include "GameClient/ProjectileComponents.h"

void transform::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using ProjectileQuery = ecs::query::Include<
		eng::TransformComponent, 
		const projectile::StateComponent, 
		const projectile::SettingsComponent>;
	for (const ecs::Entity& entity : world.Query<ProjectileQuery>())
	{
		const auto& settingsComponent = world.GetComponent<const projectile::SettingsComponent>(entity);
		const auto& stateComponent = world.GetComponent<const projectile::StateComponent>(entity);

		auto& transformComponent = world.GetComponent<eng::TransformComponent>(entity);
		transformComponent.m_Translate = settingsComponent.m_Trajectory.AtDistance(stateComponent.m_Distance);
		transformComponent.m_Translate *= settingsComponent.m_Scale;
		transformComponent.m_Translate += settingsComponent.m_Origin;
	}
}
