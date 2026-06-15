#include "EnginePCH.h"
#include "Engine/FileDropSystem.h"

#include "Engine/WindowManager.h"
#include "Engine/Window.h"
#include "Engine/FileDropEvent.h"
#include "ECS/WorldView.h"
#include "ECS/QueryTypes.h"
#include "ECS/EntityWorld.h"

void eng::FileDropSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Array<str::Path> files;
	auto& manager = world.WriteResource<eng::WindowManager>();
	manager.ConsumeFiles(files);
	if (!files.IsEmpty())
	{
		auto& event = world.AddEvent<eng::FileDropEvent>();
		event.m_Files = std::move(files);
	}
}
