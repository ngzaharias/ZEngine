#include "EditorPCH.h"
#include "Editor/Editor.h"

#include "AssetBrowser/AssetBrowserRegisterModule.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/AssetReloadSystem.h"
#include "Editor/EditorRenderAxesSystem.h"
#include "Editor/EditorRenderGridSystem.h"
#include "Editor/EditorToolbarSystem.h"
#include "Editor/GizmoCrosshairSystem.h"
#include "Editor/GizmoTransformComponent.h"
#include "Editor/GizmoTransformSystem.h"
#include "Editor/OverlaySystem.h"
#include "Editor/SettingsLocalComponent.h"
#include "Editor/SettingsLocalSystem.h"
#include "Editor/SettingsMenuSystem.h"
#include "Editor/SettingsWindowComponent.h"
#include "EntityEditor/EntityEditorRegisterModule.h"
#include "FlipbookEditor/FlipbookEditorRegisterModule.h"
#include "GameState/GameStateRegisterModule.h"
#include "Icons/IconsRegisterModule.h"
#include "InputEditor/InputEditorRegisterModule.h"
#include "SpellEditor/SpellEditorRegisterModule.h"
#include "SpriteEditor/SpriteEditorRegisterModule.h"
#include "TableEditor/TableEditorRegisterModule.h"
#include "TextureEditor/TextureEditorRegisterModule.h"
#include "ThemeEditor/ThemeEditorRegisterModule.h"
#include "TrajectoryEditor/TrajectoryEditorRegisterModule.h"

editor::Editor::Editor(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
	: m_ClientWorld(clientWorld)
	, m_ServerWorld(serverWorld)
{
}

void editor::Editor::Register()
{
	PROFILE_FUNCTION();

	m_ClientWorld.RegisterComponent<editor::gizmo::TransformComponent>();
	m_ClientWorld.RegisterComponent<editor::settings::WindowComponent>();
	m_ClientWorld.RegisterComponent<editor::settings::LocalComponent>();
	m_ClientWorld.RegisterSystem<editor::AssetReloadSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::CrosshairSystem>();
	m_ClientWorld.RegisterSystem<editor::gizmo::TransformSystem>();
	m_ClientWorld.RegisterSystem<editor::OverlaySystem>();
	m_ClientWorld.RegisterSystem<editor::RenderAxesSystem>();
	m_ClientWorld.RegisterSystem<editor::RenderGridSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::LocalSystem>();
	m_ClientWorld.RegisterSystem<editor::settings::MenuSystem>();
	m_ClientWorld.RegisterSystem<editor::ToolbarSystem>();

	editor::assets::RegisterModule(m_ClientWorld);
	editor::entity::RegisterModule(m_ClientWorld);
	editor::flipbook::RegisterModule(m_ClientWorld);
	editor::input::RegisterModule(m_ClientWorld);
	editor::spell::RegisterModule(m_ClientWorld);
	editor::sprite::RegisterModule(m_ClientWorld);
	editor::table::RegisterModule(m_ClientWorld);
	editor::texture::RegisterModule(m_ClientWorld);
	editor::theme::RegisterModule(m_ClientWorld);
	editor::trajectory::RegisterModule(m_ClientWorld);
	gamestate::RegisterModule(m_ClientWorld);
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