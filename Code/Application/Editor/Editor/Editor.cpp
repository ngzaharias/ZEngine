#include "EditorPCH.h"
#include "Editor/Editor.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/AssetBrowserSystem.h"
#include "Editor/AssetBrowserWindowComponent.h"
#include "Editor/AssetReloadSystem.h"
#include "Editor/EditorRenderAxesSystem.h"
#include "Editor/EditorRenderGridSystem.h"
#include "Editor/EntityEditorSystem.h"
#include "Editor/EntitySaveComponent.h"
#include "Editor/EntitySelectSingleton.h"
#include "Editor/EntitySelectSystem.h"
#include "Editor/EntityWindowComponent.h"
#include "Editor/FlipbookAssetOpenComponent.h"
#include "Editor/FlipbookAssetSaveComponent.h"
#include "Editor/FlipbookBatchingComponent.h"
#include "Editor/FlipbookEditorSystem.h"
#include "Editor/FlipbookWindowComponent.h"
#include "Editor/GizmoCrosshairSystem.h"
#include "Editor/GizmoTransformSystem.h"
#include "Editor/InputSystem.h"
#include "Editor/InputWindowComponent.h"
#include "Editor/OverlaySystem.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Editor/SettingsLocalSystem.h"
#include "Editor/SettingsMenuSystem.h"
#include "Editor/SettingsWindowComponent.h"
#include "Editor/TableEditorSystem.h"
#include "Editor/TableWindowComponent.h"
#include "Editor/TextureAssetImportComponent.h"
#include "Editor/TextureAssetNewComponent.h"
#include "Editor/TextureAssetOpenComponent.h"
#include "Editor/TextureAssetSaveComponent.h"
#include "Editor/TextureEditorSystem.h"
#include "Editor/TextureWindowComponent.h"
#include "Editor/TrajectoryAssetOpenComponent.h"
#include "Editor/TrajectoryAssetSaveComponent.h"
#include "Editor/TrajectoryEditorSystem.h"
#include "Editor/TrajectoryWindowComponent.h"
#include "SpriteEditor/RegisterModule.h"

editor::Editor::Editor(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void editor::Editor::Register()
{
	PROFILE_FUNCTION();

	m_ClientWorld.RegisterComponent<editor::AssetBrowserWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::EntitySaveComponent>();
	m_ClientWorld.RegisterComponent<editor::EntityWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookBatchingComponent>();
	m_ClientWorld.RegisterComponent<editor::FlipbookWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::InputWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::settings::WindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TableWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetImportComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetNewComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TextureWindowComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::TrajectoryWindowComponent>();

	m_ClientWorld.RegisterSingleton<editor::EntitySelectSingleton>();
	m_ClientWorld.RegisterSingleton<editor::settings::LocalSingleton>();

	m_ClientWorld.RegisterSystem<editor::AssetBrowserSystem>();
	m_ClientWorld.RegisterSystem<editor::AssetReloadSystem>();
	m_ClientWorld.RegisterSystem<editor::EntityEditorSystem>(m_ClientWorld);
	m_ClientWorld.RegisterSystem<editor::EntitySelectSystem>();
	m_ClientWorld.RegisterSystem<editor::FlipbookEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::CrosshairSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::TransformSystem>();
	m_ClientWorld.RegisterSystem<editor::InputSystem>();
	m_ClientWorld.RegisterSystem<editor::OverlaySystem>();
	m_ClientWorld.RegisterSystem<editor::RenderAxesSystem>();
	m_ClientWorld.RegisterSystem<editor::RenderGridSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::LocalSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::MenuSystem>();
	m_ClientWorld.RegisterSystem<editor::TableEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::TextureEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::TrajectoryEditorSystem>();

	editor::sprite::RegisterModule(m_ClientWorld);
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