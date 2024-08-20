#include "EnginePCH.h"
#include "Engine/Application.h"

#include "Core/GameTime.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraPrototype.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookPrototype.h"
#include "Engine/FontAsset.h"
#include "Engine/GLFW/Window.h"
#include "Engine/LightPrototypes.h"
#include "Engine/MusicAsset.h"
#include "Engine/NetworkComponents.h"
#include "Engine/NetworkManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsMaterialAsset.h"
#include "Engine/PhysicsPrototype.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RegisterComponents.h"
#include "Engine/RegisterSystems.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SoundAssets.h"
#include "Engine/SpriteAsset.h"
#include "Engine/SpritePrototype.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshPrototype.h"
#include "Engine/TextPrototype.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TrajectoryAsset.h"
#include "Engine/TransformPrototype.h"
#include "Engine/VoxelChunkPrototype.h"

#include <optick.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <SFML/System/Clock.hpp>

#include <iostream>
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
	const str::Name strPhysics = NAME("Physics");
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
	, m_NetworkManager(m_ComponentSerializer)
	, m_PhysicsManager()
	, m_PrototypeManager()
	, m_ComponentSerializer()
	, m_Window(nullptr)
{
	srand((unsigned int)time(NULL));

	{
		eng::WindowConfig windowConfig;
		windowConfig.m_Name = "Window A";
		windowConfig.m_Position = Vector2u::Zero;
		windowConfig.m_Size = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		m_Window = new glfw::Window(windowConfig);
	}
}

eng::Application::~Application()
{
	delete m_Window;
}

void eng::Application::Execute(int argc, char* argv[])
{
	bool isWaitingForProfiler = false;

	InitializeYojimbo();
	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);

	Register();
	Initialise();

	GameTime gameTime;

	double currTime = 0.0;
	double lastTime = 0.0;
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

			m_Window->PreUpdate(gameTime);
			if (m_Window->ShouldClose())
				break;

			Update(gameTime);

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
		m_PrototypeManager.RegisterPrototype<eng::CameraPrototype, eng::CameraLoader>(strCamera);
		m_PrototypeManager.RegisterPrototype<eng::FlipbookPrototype, eng::FlipbookLoader>(strFlipbook);
		m_PrototypeManager.RegisterPrototype<eng::AmbientLightPrototype, eng::LightLoader>(strAmbientLight);
		m_PrototypeManager.RegisterPrototype<eng::DirectionalLightPrototype, eng::LightLoader>(strDirectionalLight);
		m_PrototypeManager.RegisterPrototype<eng::PhysicsPrototype, eng::PhysicsLoader>(strPhysics, m_AssetManager, m_PhysicsManager);
		m_PrototypeManager.RegisterPrototype<eng::PointLightPrototype, eng::LightLoader>(strPointLight);
		m_PrototypeManager.RegisterPrototype<eng::SpritePrototype, eng::SpriteLoader>(strSprite);
		m_PrototypeManager.RegisterPrototype<eng::StaticMeshPrototype, eng::StaticMeshLoader>(strStaticMesh);
		m_PrototypeManager.RegisterPrototype<eng::TextPrototype, eng::TextLoader>(strText);
		m_PrototypeManager.RegisterPrototype<eng::TransformPrototype, eng::TransformLoader>(strTransform);
		m_PrototypeManager.RegisterPrototype<eng::VoxelChunkPrototype, eng::VoxelChunkLoader>(strVoxelChunk);
	}
}

void eng::Application::Initialise()
{
	PROFILE_FUNCTION();

	m_Window->Initialize();
	m_AssetManager.Initialise();
	m_PhysicsManager.Initialise();

	static sf::Music music;
	const auto* musicAsset = m_AssetManager.LoadAsset<eng::MusicAsset>(GUID("d193864e-e4f4-44e7-b6aa-bb84b95bcc5b"));
	const str::Path filepath = str::Path(str::EPath::Assets, musicAsset->m_SourceFile);
	music.openFromFile(filepath.ToChar());
	music.play();
}

void eng::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_NetworkManager.Update(gameTime);
}

void eng::Application::Shutdown()
{
	PROFILE_FUNCTION();

	m_PhysicsManager.Shutdown();
	m_AssetManager.Shutdown();
	m_Window->Shutdown();
}
