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
#include "Editor/GizmoCrosshairSystem.h"
#include "Editor/GizmoTransformSystem.h"
#include "Editor/OverlaySystem.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Editor/SettingsLocalSystem.h"
#include "Editor/SettingsMenuSystem.h"
#include "Editor/SettingsWindowComponent.h"
#include "InputEditor/RegisterModule.h"
#include "FlipbookEditor/RegisterModule.h"
#include "SpriteEditor/RegisterModule.h"
#include "TableEditor/RegisterModule.h"
#include "TextureEditor/RegisterModule.h"
#include "TrajectoryEditor/RegisterModule.h"

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
	m_ClientWorld.RegisterComponent<editor::settings::WindowComponent>();

	m_ClientWorld.RegisterSingleton<editor::EntitySelectSingleton>();
	m_ClientWorld.RegisterSingleton<editor::settings::LocalSingleton>();

	m_ClientWorld.RegisterSystem<editor::AssetBrowserSystem>();
	m_ClientWorld.RegisterSystem<editor::AssetReloadSystem>();
	m_ClientWorld.RegisterSystem<editor::EntityEditorSystem>(m_ClientWorld);
	m_ClientWorld.RegisterSystem<editor::EntitySelectSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::CrosshairSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::TransformSystem>();
	m_ClientWorld.RegisterSystem<editor::OverlaySystem>();
	m_ClientWorld.RegisterSystem<editor::RenderAxesSystem>();
	m_ClientWorld.RegisterSystem<editor::RenderGridSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::LocalSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::MenuSystem>();

	editor::flipbook::RegisterModule(m_ClientWorld);
	editor::input::RegisterModule(m_ClientWorld);
	editor::sprite::RegisterModule(m_ClientWorld);
	editor::table::RegisterModule(m_ClientWorld);
	editor::texture::RegisterModule(m_ClientWorld);
	editor::trajectory::RegisterModule(m_ClientWorld);
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