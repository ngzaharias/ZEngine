#include "HiddenPCH.h"
#include "ServerHidden/HiddenRevealSystem.h"

#include "Core/Profiler.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "SharedHidden/HiddenRevealedEvent.h"
#include "SharedHidden/HiddenSelectedEvent.h"

void server::hidden::RevealSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& selectedData : world.Events<const shared::hidden::SelectedEvent>())
	{
		auto& revealedData = world.AddEvent<shared::hidden::RevealedEvent>();
		revealedData.m_Entity = selectedData.m_Entity;
	}
}