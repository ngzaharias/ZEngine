#include "EditorPCH.h"
#include "Editor/Editor.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Editor/FlipbookEditor.h"

editor::Editor::Editor(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void editor::Editor::Register()
{
	PROFILE_FUNCTION();

	m_ClientWorld.RegisterComponent<editor::FlipbookAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookBatchingComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookWindowComponent>();

	m_ClientWorld.RegisterSystem<editor::FlipbookEditor>();
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