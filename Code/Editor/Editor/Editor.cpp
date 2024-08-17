#include "EditorPCH.h"
#include "Editor/Editor.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/FlipbookEditor.h"
#include "Editor/TableEditor.h"
#include "Editor/SpriteEditor.h"
#include "Editor/TextureEditor.h"
#include "Editor/TrajectoryEditor.h"
#include "Editor/TransformEditor.h"

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
	m_ClientWorld.RegisterComponent<editor::SpriteAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TableWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetImportComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TransformAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TransformWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryWindowComponent>();

	m_ClientWorld.RegisterSystem<editor::FlipbookEditor>();
	m_ClientWorld.RegisterSystem<editor::TableEditor>();
	m_ClientWorld.RegisterSystem<editor::SpriteEditor>();
	m_ClientWorld.RegisterSystem<editor::TextureEditor>();
	m_ClientWorld.RegisterSystem<editor::TrajectoryEditor>();
	m_ClientWorld.RegisterSystem<editor::TransformEditor>();
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