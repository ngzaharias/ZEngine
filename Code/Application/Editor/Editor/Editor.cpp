#include "EditorPCH.h"
#include "Editor/Editor.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/AssetBrowserSystem.h"
#include "Editor/AssetReloadSystem.h"
#include "Editor/CameraSystem.h"
#include "Editor/EntityEditorSystem.h"
#include "Editor/EntitySelectComponent.h"
#include "Editor/EntitySelectSystem.h"
#include "Editor/FlipbookEditorSystem.h"
#include "Editor/GizmoCrosshairSystem.h"
#include "Editor/GizmoTransformSystem.h"
#include "Editor/InputSystem.h"
#include "Editor/OverlaySystem.h"
#include "Editor/RenderStage_Axes.h"
#include "Editor/RenderStage_Grid.h"
#include "Editor/SettingsMenuSystem.h"
#include "Editor/SettingsComponents.h"
#include "Editor/SettingsLocalSystem.h"
#include "Editor/SpriteEditorSystem.h"
#include "Editor/TableEditorSystem.h"
#include "Editor/TextureEditorSystem.h"
#include "Editor/TrajectoryEditorSystem.h"
#include "Engine/RenderSystem.h"

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
	m_ClientWorld.RegisterComponent<editor::SpriteAssetNewComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteAssetOpenComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteAssetSaveComponent>();
	m_ClientWorld.RegisterComponent<editor::SpriteBatchingComponent>();
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

	m_ClientWorld.RegisterSingleton<editor::EntitySelectSingleton>();
	m_ClientWorld.RegisterSingleton<editor::settings::LocalSingleton>();

	m_ClientWorld.RegisterSystem<editor::AssetBrowserSystem>();
	m_ClientWorld.RegisterSystem<editor::AssetReloadSystem>();
	m_ClientWorld.RegisterSystem<editor::CameraSystem>();
	m_ClientWorld.RegisterSystem<editor::EntityEditorSystem>(m_ClientWorld);
	m_ClientWorld.RegisterSystem<editor::EntitySelectSystem>();
	m_ClientWorld.RegisterSystem<editor::FlipbookEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::CrosshairSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::TransformSystem>();
	m_ClientWorld.RegisterSystem<editor::InputSystem>();
	m_ClientWorld.RegisterSystem<editor::OverlaySystem>();
	m_ClientWorld.RegisterSystem<editor::settings::LocalSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::MenuSystem>();
	m_ClientWorld.RegisterSystem<editor::SpriteEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::TableEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::TextureEditorSystem>();
	m_ClientWorld.RegisterSystem<editor::TrajectoryEditorSystem>();

	// needs to run before the render system but after the camera systems
	m_ClientWorld.RegisterSystemPriority<editor::gizmo::CrosshairSystem>(4999);

	// #todo: find a different place for this
	{
		auto& renderSystem = m_ClientWorld.GetSystem<eng::RenderSystem>();
		renderSystem.RegisterStage<editor::RenderStage_Grid>();
		renderSystem.RegisterStage<editor::RenderStage_Axes>();
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