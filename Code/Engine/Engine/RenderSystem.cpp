#include "EnginePCH.h"
#include "Engine/RenderSystem.h"

#include <Core/GameTime.h>
#include <Core/Map.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Engine/AssetManager.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/RenderStage_ImGui.h"
#include "Engine/RenderStage_Lines.h"
#include "Engine/RenderStage_Opaque.h"
#include "Engine/RenderStage_Shadow.h"
#include "Engine/RenderStage_Translucent.h"
#include "Engine/RenderStage_UI.h"
#include "Engine/RenderStage_Voxels.h"

#include "Engine/GLFW/Window.h"

#include <GLFW/glfw3.h>

// http://realtimecollisiondetection.net/blog/?p=86
// https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
// https://gamedevelopment.tutsplus.com/articles/gamma-correction-and-why-it-matters--gamedev-14466

eng::RenderSystem::RenderSystem(
	ecs::EntityWorld& entityWorld,
	glfw::Window& window)
	: m_EntityWorld(entityWorld)
	, m_Window(window)
{
	auto& assetManager = entityWorld.GetResource<eng::AssetManager>();
	m_RenderStages.Append(new eng::RenderStage_Shadow(assetManager));
	m_RenderStages.Append(new eng::RenderStage_Voxels(assetManager));
	m_RenderStages.Append(new eng::RenderStage_Opaque(assetManager));
	m_RenderStages.Append(new eng::RenderStage_Lines(assetManager));
	m_RenderStages.Append(new eng::RenderStage_Translucent(assetManager));
	m_RenderStages.Append(new eng::RenderStage_UI(assetManager));
	m_RenderStages.Append(new eng::RenderStage_ImGui(assetManager, window));
}

eng::RenderSystem::~RenderSystem()
{
	for (RenderStage* stage : m_RenderStages)
		delete stage;
	m_RenderStages.RemoveAll();
}

void eng::RenderSystem::Initialise(World& world)
{
	for (auto&& stage : m_RenderStages)
		stage->Initialise(m_EntityWorld);
}

void eng::RenderSystem::Shutdown(World& world)
{
	for (auto&& stage : m_RenderStages)
		stage->Shutdown(m_EntityWorld);
}

void eng::RenderSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	{
		constexpr float s_Distance = 100000.f;

		auto& linesComponent = world.GetSingleton<eng::LinesComponent>();
		linesComponent.AddLine(Vector3f::AxisX * -s_Distance, Vector3f::AxisX * s_Distance, Vector4f(1.f, 0.f, 0.f, 1.f));
		linesComponent.AddLine(Vector3f::AxisY * -s_Distance, Vector3f::AxisY * s_Distance, Vector4f(0.f, 1.f, 0.f, 1.f));
		linesComponent.AddLine(Vector3f::AxisZ * -s_Distance, Vector3f::AxisZ * s_Distance, Vector4f(0.f, 0.f, 1.f, 1.f));
	}

	for (auto&& stage : m_RenderStages)
		stage->Render(m_EntityWorld);
}
