#include "EditorPCH.h"
#include "Editor/Application.h"

#include "Editor/AssetBrowser.h"
#include "Editor/EntityBrowser.h"
#include "Editor/ExampleComponent.h"
#include "Editor/FlipbookEditor.h"
#include "Editor/Historian.h"
#include "Editor/Inspector.h"
#include "Editor/MainMenuBar.h"
#include "Editor/NodeGraphEditor.h"
#include "Editor/SceneEditor.h"
#include "Editor/SpriteEditor.h"
#include "Editor/SpriteExtractor.h"
#include "Editor/TemplateEditor.h"

#include <Engine/AssetManager.h>
#include <Engine/CameraComponent.h>
#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/GameTime.h>
#include <Engine/LevelSystem.h>
#include <Engine/NameComponent.h>
#include <Engine/PhysicsSystem.h>
#include <Engine/Screen.h>
#include <Engine/SoundSystem.h>
#include <Engine/SpriteComponent.h>
#include <Engine/Texture2DAsset.h>
#include <Engine/TransformComponent.h>

#include <imgui/imgui.h>

namespace
{
	const str::Guid strDefaultFlipbook = GUID("9cf3dbdc-769e-a09a-6e4a-a0390e246666");
	const str::Guid strDefaultMaterial = GUID("a4835493-ae5a-40ba-8083-06deb381c801");
	const str::Guid strDefaultSound = GUID("18fa78b3-6f37-4680-81b4-951747f76f3a");
	const str::Guid strDefaultSprite = GUID("52ffdca6-bc1d-6423-0eda-0e2056e9662b");
	const str::Guid strDefaultTexture = GUID("5ed3b860-d64c-459a-a188-80dc7cc0e85f");
}

editor::Application::Application()
{
}

editor::Application::~Application()
{
}

void editor::Application::Register()
{
	core::Application::Register();

	//// components
	//m_EntityWorld.RegisterComponent<example::ExampleComponent>();

	//// systems
	//m_EntityWorld.RegisterSystem<editor::Historian>();

	//m_EntityWorld.RegisterSystem<editor::TemplateEditor>(
	//	m_AssetManager);

	//m_EntityWorld.RegisterSystem<editor::FlipbookEditor>(
	//	m_AssetManager);

	//m_EntityWorld.RegisterSystem<editor::SceneEditor>(
	//	*m_Window);

	//m_EntityWorld.RegisterSystem<editor::SpriteEditor>(
	//	m_AssetManager);

	//m_EntityWorld.RegisterSystem<editor::SpriteExtractor>(
	//	m_AssetManager);

	//m_EntityWorld.RegisterSystem<editor::AssetBrowser>(
	//	m_AssetManager,
	//	m_EntityWorld.GetSystem<editor::FlipbookEditor>(),
	//	m_EntityWorld.GetSystem<editor::SpriteEditor>(),
	//	m_EntityWorld.GetSystem<editor::SpriteExtractor>());

	//m_EntityWorld.RegisterSystem<editor::Inspector>(
	//	m_EntityWorld.GetSystem<editor::Historian>(),
	//	m_EntityWorld.GetSystem<editor::SceneEditor>());

	//m_EntityWorld.RegisterSystem<editor::EntityBrowser>(
	//	m_EntityWorld.GetSystem<editor::Inspector>());

	//m_EntityWorld.RegisterSystem<editor::NodeGraphEditor>();

	//m_EntityWorld.RegisterSystem<editor::MainMenuBar>(
	//	m_EntityWorld.GetSystem<audio::SoundSystem>(),
	//	m_EntityWorld.GetSystem<editor::AssetBrowser>(),
	//	m_EntityWorld.GetSystem<editor::EntityBrowser>(),
	//	m_EntityWorld.GetSystem<editor::Historian>(),
	//	m_EntityWorld.GetSystem<editor::Inspector>(),
	//	m_EntityWorld.GetSystem<editor::NodeGraphEditor>(),
	//	m_EntityWorld.GetSystem<editor::TemplateEditor>());

}

void editor::Application::Initialise()
{
	core::Application::Initialise();

	//physics::PhysicsSystem& physicsSystem = m_EntityWorld.GetSystem<physics::PhysicsSystem>();
	//entt::sink(physicsSystem.m_OnCollideSignal).connect<&editor::Application::PlaySound>(this);

	//core::LevelSystem& levelSystem = m_EntityWorld.GetSystem<core::LevelSystem>();
	//levelSystem.Load(str::Path("Assets/Levels/Default/"));

	//// example entity
	//{
	//	ecs::Entity entity = m_EntityWorld.CreateEntity();
	//	auto& exampleComponent = m_EntityWorld.AddComponent<example::ExampleComponent>(entity);
	//	exampleComponent.m_PhysicsMaterial = strDefaultMaterial;
	//	exampleComponent.m_Sound = strDefaultSound;
	//	exampleComponent.m_Texture = strDefaultTexture;
	//	auto& nameComponent = m_EntityWorld.AddComponent<core::NameComponent>(entity);
	//	nameComponent.m_Name = "Example";
	//}
}

void editor::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	core::Application::Update(gameTime);
}

void editor::Application::Destroy()
{
	return core::Application::Destroy();
}

void editor::Application::PlaySound()
{
	//m_EntityWorld.GetSystem<audio::SoundSystem>().PlaySound(strDefaultSound);
}
