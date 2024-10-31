#include "EnginePCH.h"
#include "Engine/Application.h"

#include "Core/GameTime.h"
#include "Engine/AchievementTable.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FontAsset.h"
#include "Engine/GLFW/Window.h"
#include "Engine/LightComponents.h"
#include "Engine/MusicAsset.h"
#include "Engine/NetworkComponents.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsMaterialAsset.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SoundAssets.h"
#include "Engine/SpriteAsset.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TextComponent.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TrajectoryAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/VoxelComponents.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <optick.h>
#include <random>
#include <time.h>

namespace
{
	const str::Name strAmbientLight = NAME("AmbientLight");
	const str::Name strCamera = NAME("Camera");
	const str::Name strDirectionalLight = NAME("DirectionalLight");
	const str::Name strExample = NAME("Example");
	const str::Name strFont = NAME("Font");
	const str::Name strFlipbook = NAME("Flipbook");
	const str::Name strMusic = NAME("Music");
	const str::Name strPhysicsMaterial = NAME("PhysicsMaterial");
	const str::Name strPointLight = NAME("PointLight");
	const str::Name strShader = NAME("Shader");
	const str::Name strSoundRandom = NAME("SoundRandom");
	const str::Name strSoundSequence = NAME("SoundSequence");
	const str::Name strSoundSingle = NAME("SoundSingle");
	const str::Name strSprite = NAME("Sprite");
	const str::Name strStaticMesh = NAME("StaticMesh");
	const str::Name strTemplate = NAME("Template");
	const str::Name strText = NAME("Text");
	const str::Name strTexture2D = NAME("Texture2D");
	const str::Name strTrajectory = NAME("Trajectory");
	const str::Name strTransform = NAME("Transform");
	const str::Name strVoxelChunk = NAME("VoxelChunk");
}

eng::Application::Application()
	: m_Window(nullptr)
	, m_AssetManager()
	, m_ImguiManager()
	, m_NetworkManager(m_ComponentSerializer)
	, m_PhysicsManager()
	, m_PlatformManager()
	, m_PrototypeManager()
	, m_ComponentSerializer()
{
	srand((unsigned int)time(NULL));
	m_Window = new glfw::Window(m_WindowConfig);
}

eng::Application::~Application()
{
}

void eng::Application::Execute(int argc, char* argv[])
{
	InitializeYojimbo();
	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);

	Register();
	Initialise();

	GameTime gameTime;

	double currTime = 0.0;
	double lastTime = 0.0;
	bool isWaitingForProfiler = false;
	while (true)
	{
		PROFILE_TICK("MainThread");

		// profiling
		{
			if (isWaitingForProfiler && !profile::IsActive())
				continue;

			isWaitingForProfiler = false;
		}

		sf::Time elapsed = clock.restart();
		gameTime.m_DeltaTime = elapsed.asSeconds();
		gameTime.m_TotalTime += gameTime.m_DeltaTime;
		gameTime.m_Frame++;

		{
			PROFILE_CUSTOM("eng::Application::Execute");

			m_Window->PreUpdate(gameTime);
			if (m_Window->ShouldClose())
				break;

			PreUpdate(gameTime);
			Update(gameTime);
			PostUpdate(gameTime);

			m_Window->PostUpdate(gameTime);
		}
	}

	Shutdown();

	ShutdownYojimbo();
}

void eng::Application::Register()
{
	// assets
	{
		m_AssetManager.RegisterAsset<eng::FlipbookAsset, eng::FlipbookAssetLoader>(strFlipbook);
		m_AssetManager.RegisterAsset<eng::FontAsset, eng::FontAssetLoader>(strFont);
		m_AssetManager.RegisterAsset<eng::MusicAsset, eng::MusicAssetLoader>(strMusic);
		m_AssetManager.RegisterAsset<eng::PhysicsMaterialAsset, eng::PhysicsMaterialAssetLoader>(strPhysicsMaterial, m_PhysicsManager);
		m_AssetManager.RegisterAsset<eng::ShaderAsset, eng::ShaderAssetLoader>(strShader);
		m_AssetManager.RegisterAsset<eng::sound::RandomAsset, eng::sound::AssetLoader>(strSoundRandom);
		m_AssetManager.RegisterAsset<eng::sound::SequenceAsset, eng::sound::AssetLoader>(strSoundSequence);
		m_AssetManager.RegisterAsset<eng::sound::SingleAsset, eng::sound::AssetLoader>(strSoundSingle);
		m_AssetManager.RegisterAsset<eng::SpriteAsset, eng::SpriteAssetLoader>(strSprite);
		m_AssetManager.RegisterAsset<eng::StaticMeshAsset, eng::StaticMeshAssetLoader>(strStaticMesh);
		m_AssetManager.RegisterAsset<eng::Texture2DAsset, eng::Texture2DAssetLoader>(strTexture2D);
		m_AssetManager.RegisterAsset<eng::TrajectoryAsset, eng::TrajectoryAssetLoader>(strTrajectory);
	}

	// prototypes
	{
		m_PrototypeManager.Register<eng::camera::Bound2DComponent>();
		m_PrototypeManager.Register<eng::camera::Move2DComponent>();
		m_PrototypeManager.Register<eng::camera::Move3DComponent>();
		m_PrototypeManager.Register<eng::camera::Pan3DComponent>();
		m_PrototypeManager.Register<eng::camera::ProjectionComponent>();
		m_PrototypeManager.Register<eng::camera::Zoom2DComponent>();
		m_PrototypeManager.Register<eng::FlipbookComponent>();
		m_PrototypeManager.Register<eng::LightAmbientComponent>();
		m_PrototypeManager.Register<eng::LightDirectionalComponent>();
		m_PrototypeManager.Register<eng::LightPointComponent>();
		m_PrototypeManager.Register<eng::PhysicsComponent>();
		m_PrototypeManager.Register<eng::SpriteComponent>();
		m_PrototypeManager.Register<eng::StaticMeshComponent>();
		m_PrototypeManager.Register<eng::TextComponent>();
		m_PrototypeManager.Register<eng::TransformComponent>();
		m_PrototypeManager.Register<voxel::ChunkComponent>();
	}

	// tables
	{
		m_TableHeadmaster.Register<eng::AchievementTable>("Achievements");
	}
}

void eng::Application::Initialise()
{
	PROFILE_FUNCTION();

	m_Window->Initialize();
	m_AssetManager.Initialise();
	m_ImguiManager.Initialise(*m_Window);
	m_PhysicsManager.Initialise();
	m_PlatformManager.Initialise();
	m_TableHeadmaster.Initialise(str::Path(str::EPath::Assets, "Tables"));
}

void eng::Application::PreUpdate(const GameTime& gameTime)
{
	m_PlatformManager.Update(gameTime);
	m_ImguiManager.PreUpdate();
}

void eng::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_NetworkManager.Update(gameTime);
}

void eng::Application::PostUpdate(const GameTime& gameTime)
{
	m_ImguiManager.PostUpdate();
}

void eng::Application::Shutdown()
{
	PROFILE_FUNCTION();

	m_TableHeadmaster.Shutdown();
	m_PlatformManager.Shutdown();
	m_PhysicsManager.Shutdown();
	m_ImguiManager.Shutdown();
	m_AssetManager.Shutdown();
	m_Window->Shutdown();

	delete m_Window;
}