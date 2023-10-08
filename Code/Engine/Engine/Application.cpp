#include "EnginePCH.h"
#include "Engine/Application.h"

#include <Core/GameTime.h>

#include "Engine/AssetManager.h"
#include "Engine/CameraPrototype.h"
#include "Engine/FileHelpers.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookPrototype.h"
#include "Engine/FontAsset.h"
#include "Engine/GLFW/Window.h"
#include "Engine/LightPrototypes.h"
#include "Engine/NameComponent.h"
#include "Engine/NamePrototype.h"
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
#include "Engine/SoundAsset.h"
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
	const str::Name strName = NAME("Name");
	const str::Name strPhysics = NAME("Physics");
	const str::Name strPhysicsMaterial = NAME("PhysicsMaterial");
	const str::Name strPointLight = NAME("PointLight");
	const str::Name strShader = NAME("Shader");
	const str::Name strSound = NAME("Sound");
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
	, m_PrototypeManager(m_AssetManager)
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
		m_AssetManager.RegisterAsset<eng::PhysicsMaterialAsset, eng::PhysicsMaterialAssetLoader>(strPhysicsMaterial, m_PhysicsManager);
		m_AssetManager.RegisterAsset<eng::ShaderAsset, eng::ShaderAssetLoader>(strShader);
		m_AssetManager.RegisterAsset<eng::SoundAsset, eng::SoundAssetLoader>(strSound);
		m_AssetManager.RegisterAsset<eng::SpriteAsset, eng::SpriteAssetLoader>(strSprite);
		m_AssetManager.RegisterAsset<eng::StaticMeshAsset, eng::StaticMeshAssetLoader>(strStaticMesh);
		m_AssetManager.RegisterAsset<eng::Texture2DAsset, eng::Texture2DAssetLoader>(strTexture2D);
		m_AssetManager.RegisterAsset<eng::TrajectoryAsset, eng::TrajectoryAssetLoader>(strTrajectory);
	}

	// prototypes
	{
		m_PrototypeManager.RegisterPrototype<eng::CameraPrototype, eng::CameraPrototypeLoader>(strCamera);
		m_PrototypeManager.RegisterPrototype<eng::FlipbookPrototype, eng::FlipbookPrototypeLoader>(strFlipbook);
		m_PrototypeManager.RegisterPrototype<eng::AmbientLightPrototype, eng::LightPrototypeLoader>(strAmbientLight);
		m_PrototypeManager.RegisterPrototype<eng::DirectionalLightPrototype, eng::LightPrototypeLoader>(strDirectionalLight);
		m_PrototypeManager.RegisterPrototype<eng::NamePrototype, eng::NamePrototypeLoader>(strName);
		m_PrototypeManager.RegisterPrototype<eng::PhysicsPrototype, eng::PhysicsPrototypeLoader>(strPhysics, m_PhysicsManager);
		m_PrototypeManager.RegisterPrototype<eng::PointLightPrototype, eng::LightPrototypeLoader>(strPointLight);
		m_PrototypeManager.RegisterPrototype<eng::SpritePrototype, eng::SpritePrototypeLoader>(strSprite);
		m_PrototypeManager.RegisterPrototype<eng::StaticMeshPrototype, eng::StaticMeshPrototypeLoader>(strStaticMesh);
		m_PrototypeManager.RegisterPrototype<eng::TextPrototype, eng::TextPrototypeLoader>(strText);
		m_PrototypeManager.RegisterPrototype<eng::TransformPrototype, eng::TransformPrototypeLoader>(strTransform);
		m_PrototypeManager.RegisterPrototype<eng::VoxelChunkPrototype, eng::VoxelChunkPrototypeLoader>(strVoxelChunk);
	}
}

void eng::Application::Initialise()
{
	PROFILE_FUNCTION();
	m_Window->Initialize();
	m_AssetManager.Initialise();
	m_NetworkManager.Initialise();
	m_PhysicsManager.Initialise();
	m_PrototypeManager.Initialise();
}

void eng::Application::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_NetworkManager.Update(gameTime);
}

void eng::Application::Shutdown()
{
	PROFILE_FUNCTION();
	m_PrototypeManager.Shutdown();
	m_PhysicsManager.Shutdown();
	m_NetworkManager.Shutdown();
	m_AssetManager.Shutdown();
	m_Window->Shutdown();
}
