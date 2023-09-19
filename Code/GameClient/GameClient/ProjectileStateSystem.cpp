#include "GameClientPCH.h"
#include "GameClient/ProjectileStateSystem.h"

#include <Core/GameTime.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/TransformComponent.h>

#include "GameClient/ProjectileComponents.h"

void projectile::StateSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<projectile::StateComponent, const projectile::SettingsComponent>>())
	{
		const auto& settingsComponent = world.GetComponent<const projectile::SettingsComponent>(entity);
		auto& stateComponent = world.GetComponent<projectile::StateComponent>(entity);

		const float velocity = stateComponent.m_Velocity / settingsComponent.m_Scale;
		stateComponent.m_Distance += velocity * gameTime.m_DeltaTime;
		stateComponent.m_Lifetime += gameTime.m_DeltaTime;

		// #note: increase velocity afterwards so that the initial speed is used for at least one frame
		if (std::holds_alternative<speed::Linear>(settingsComponent.m_Velocity))
		{
			const speed::Linear& data = std::get<speed::Linear>(settingsComponent.m_Velocity);
			stateComponent.m_Velocity += data.m_Increase * gameTime.m_DeltaTime;
			stateComponent.m_Velocity = math::Clamp(stateComponent.m_Velocity, data.m_Minimum, data.m_Maximum);
		}
	}
}
