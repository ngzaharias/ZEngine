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
#include "Engine/RenderStage_Lines.h"
#include "Engine/RenderStage_Opaque.h"
#include "Engine/RenderStage_Shadow.h"
#include "Engine/RenderStage_Translucent.h"
#include "Engine/RenderStage_UI.h"
#include "Engine/RenderStage_Voxels.h"
#include "Engine/SettingsComponents.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// http://realtimecollisiondetection.net/blog/?p=86
// https://blog.molecular-matters.com/2014/11/06/stateless-layered-multi-threaded-rendering-part-1/
// https://gamedevelopment.tutsplus.com/articles/gamma-correction-and-why-it-matters--gamedev-14466

namespace
{
	constexpr Colour s_ClearColour = Colour(0.24f);
}

eng::RenderSystem::RenderSystem(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
{
	RegisterStage<eng::RenderStage_Shadow>();
	RegisterStage<eng::RenderStage_Voxels>();
	RegisterStage<eng::RenderStage_Opaque>();
	RegisterStage<eng::RenderStage_Lines>();
	RegisterStage<eng::RenderStage_Translucent>();
	RegisterStage<eng::RenderStage_UI>();
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
		const Colour& colour = s_ClearColour;
		glClearDepthf(1.f);
		glClearColor(colour.r, colour.g, colour.b, 1.f);

		// the depth mask must be enabled BEFORE clearing the depth buffer
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	for (auto&& stage : m_RenderStages)
		stage->Render(m_EntityWorld);
}

