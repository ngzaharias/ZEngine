#include "EnginePCH.h"
#include "Engine/RenderSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/RenderStage_Cursor.h"
#include "Engine/RenderStage_ImGui.h"
#include "Engine/RenderStage_Lines.h"
#include "Engine/RenderStage_Noesis_Post.h"
#include "Engine/RenderStage_Noesis_Pre.h"
#include "Engine/RenderStage_Opaque.h"
#include "Engine/RenderStage_Shadow.h"
#include "Engine/RenderStage_Translucent.h"
#include "Engine/RenderStage_UI.h"
#include "Engine/RenderStage_Voxels.h"

// http://realtimecollisiondetection.net/blog/?p=86
// https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
// https://gamedevelopment.tutsplus.com/articles/gamma-correction-and-why-it-matters--gamedev-14466

eng::RenderSystem::RenderSystem(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
	RegisterStage<eng::RenderStage_Noesis_Pre>();
	RegisterStage<eng::RenderStage_Shadow>();
	RegisterStage<eng::RenderStage_Voxels>();
	RegisterStage<eng::RenderStage_Opaque>();
	RegisterStage<eng::RenderStage_Lines>();
	RegisterStage<eng::RenderStage_Translucent>();
	RegisterStage<eng::RenderStage_UI>();
	RegisterStage<eng::RenderStage_Noesis_Post>();
	RegisterStage<eng::RenderStage_ImGui>();
	RegisterStage<eng::RenderStage_Cursor>();
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

