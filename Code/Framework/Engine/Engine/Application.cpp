#include "EnginePCH.h"
#include "Engine/Application.h"

#include "Core/GameTime.h"
#include "Core/Profiler.h"
#include "Engine/AchievementTable.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/ColourTable.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FontAsset.h"
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
#include "Engine/ShaderAsset.h"
#include "Engine/SoundAssets.h"
#include "Engine/SpriteAsset.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TextComponent.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/ThemeTable.h"
#include "Engine/TrajectoryAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/VisibilityComponent.h"
#include "Engine/Window.h"
#include "Voxel/VoxelComponents.h"

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
	: m_AssetManager()
	, m_ImguiManager()
	, m_NetworkManager(m_ComponentSerializer)
	, m_PhysicsManager()
	, m_PlatformManager()
	, m_PrototypeManager()
	, m_WindowManager()
	, m_ComponentSerializer()
{
	srand((unsigned int)time(NULL));
}

eng::Application::~Application()
{
}

void eng::Application::Execute(int argc, char* argv[])
{
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

		lastTime = currTime;
		currTime = glfwGetTime();

		gameTime.m_DeltaTime = static_cast<float>(currTime - lastTime);
		gameTime.m_TotalTime += gameTime.m_DeltaTime;
		gameTime.m_Frame++;

		{
			PROFILE_CUSTOM("eng::Application::Execute");

			PreUpdate(gameTime);
			Update(gameTime);
			PostUpdate(gameTime);

			if (ShouldClose())
				break;
		}
	}

	Shutdown();
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
		m_PrototypeManager.Register<eng::VisibilityComponent>();
	}

	// tables
	{
		m_TableHeadmaster.Register<eng::AchievementTable>("Achievements");
		m_TableHeadmaster.Register<eng::ColourTable>("Colours");
		m_TableHeadmaster.Register<eng::ThemeTable>("Themes");
	}
}

void eng::Application::Initialise()
{
	PROFILE_FUNCTION();

	// do first
	m_WindowManager.Initialise();

	m_AssetManager.Initialise();
	m_ImguiManager.Initialise(*m_WindowManager.GetWindow(0));
	m_PhysicsManager.Initialise();
	m_PlatformManager.Initialise();
	m_TableHeadmaster.Initialise(str::Path(str::EPath::Assets, "Tables"));
}

void eng::Application::PreUpdate(const GameTime& gameTime)
{
	// do first
	m_WindowManager.PreUpdate(gameTime);

	m_AssetManager.Update();
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

	// do last
	m_WindowManager.PostUpdate(gameTime);
}

void eng::Application::Shutdown()
{
	PROFILE_FUNCTION();

	m_TableHeadmaster.Shutdown();
	m_PlatformManager.Shutdown();
	m_PhysicsManager.Shutdown();
	m_ImguiManager.Shutdown();
	m_AssetManager.Shutdown();

	// do last
	m_WindowManager.Shutdown();
}

bool eng::Application::ShouldClose()
{
	const eng::Window* window = m_WindowManager.GetWindow(0);
	return window && window->ShouldClose();
}
