#include "EditorPCH.h"
#include "Editor/Editor.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>


editor::Editor::Editor(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void editor::Editor::Register()
{
	PROFILE_FUNCTION();


}

void editor::Editor::Initialise()
{
	PROFILE_FUNCTION();
}

void editor::Editor::Shutdown()
{
	PROFILE_FUNCTION();
}

void editor::Editor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}