#include "EnginePCH.h"
#include "Engine/RenderSystem.h"

#include "Core/GameTime.h"
#include "Core/Map.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
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

eng::RenderSystem::RenderSystem(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
	m_RenderStages.Append(new eng::RenderStage_Shadow());
	m_RenderStages.Append(new eng::RenderStage_Voxels());
	m_RenderStages.Append(new eng::RenderStage_Opaque());
	m_RenderStages.Append(new eng::RenderStage_Lines());
	m_RenderStages.Append(new eng::RenderStage_Translucent());
	m_RenderStages.Append(new eng::RenderStage_UI());
	m_RenderStages.Append(new eng::RenderStage_ImGui());
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

	for (auto&& stage : m_RenderStages)
		stage->Render(m_EntityWorld);
}
