#include "EditorPCH.h"
#include "Editor/Editor.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/EntityEditor.h"
#include "Editor/FlipbookEditor.h"
#include "Editor/GizmoAxes.h"
#include "Editor/GizmoCrosshair.h"
#include "Editor/GizmoGrid.h"
#include "Editor/GizmoTransform.h"
#include "Editor/RenderStage_Grid.h"
#include "Editor/SettingsMenu.h"
#include "Editor/SettingsComponents.h"
#include "Editor/SettingsLocalSystem.h"
#include "Editor/SpriteEditor.h"
#include "Editor/TableEditor.h"
#include "Editor/TextureEditor.h"
#include "Editor/TrajectoryEditor.h"
#include "Engine/RenderSystem.h"

editor::Editor::Editor(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void editor::Editor::Register()
{
	PROFILE_FUNCTION();

	m_ClientWorld.RegisterComponent<editor::EntitySaveComponent>();
	m_ClientWorld.RegisterComponent<editor::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookBatchingComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::settings::WindowComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteAssetNewComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TableWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetImportComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetNewComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryWindowComponent>();

	m_ClientWorld.RegisterSingleton<editor::settings::LocalComponent>();

	m_ClientWorld.RegisterSystem<editor::EntityEditor>(m_ClientWorld);
	m_ClientWorld.RegisterSystem<editor::FlipbookEditor>();
	m_ClientWorld.RegisterSystem<editor::GizmoAxes>();
	m_ClientWorld.RegisterSystem<editor::GizmoCrosshair>();
	m_ClientWorld.RegisterSystem<editor::GizmoGrid>();
	m_ClientWorld.RegisterSystem<editor::GizmoTransform>();
	m_ClientWorld.RegisterSystem<editor::settings::LocalSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::MenuSystem>();
	m_ClientWorld.RegisterSystem<editor::SpriteEditor>();
	m_ClientWorld.RegisterSystem<editor::TableEditor>();
	m_ClientWorld.RegisterSystem<editor::TextureEditor>();
	m_ClientWorld.RegisterSystem<editor::TrajectoryEditor>();

	// needs to run before the render system but after the camera systems
	m_ClientWorld.RegisterSystemPriority<editor::GizmoCrosshair>(4999);

	// #todo: find a different place for this
	{
		auto& renderSystem = m_ClientWorld.GetSystem<eng::RenderSystem>();
		renderSystem.RegisterStage<editor::RenderStage_Grid>();
	}
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