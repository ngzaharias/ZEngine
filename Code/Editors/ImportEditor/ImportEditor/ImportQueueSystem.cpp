#include "ImportPCH.h"
#include "ImportEditor/ImportQueueSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/FileDropEvent.h"
#include "ImportEditor/ImportAdvanceEvent.h"
#include "ImportEditor/ImportQueueComponent.h"

void editor::importer::QueueSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const auto& event : world.Events<const eng::FileDropEvent>())
	{
		auto& queueComponent = !world.HasComponent<editor::importer::QueueComponent>()
			? world.AddComponent<editor::importer::QueueComponent>()
			: world.WriteComponent<editor::importer::QueueComponent>();
		queueComponent.m_Files = event.m_Files;
	}

	for (const auto& finishedEvent : world.Events<const editor::importer::AdvanceEvent>())
	{
		auto& queueComponent = world.WriteComponent<editor::importer::QueueComponent>();
		queueComponent.m_Files.RemoveOrderedAt(0);

		if (queueComponent.m_Files.IsEmpty())
		{
			world.RemoveComponent<editor::importer::QueueComponent>();
		}
	}
}