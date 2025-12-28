#include "RenderPCH.h"
#include "Render/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Render/RenderCursorComponent.h"
#include "Render/RenderCursorSystem.h"
#include "Render/RenderDebugComponent.h"
#include "Render/RenderDebugSystem.h"
#include "Render/RenderImGuiComponent.h"
#include "Render/RenderImGuiSystem.h"
#include "Render/RenderOpaqueComponent.h"
#include "Render/RenderOpaqueSystem.h"
#include "Render/RenderShadowComponent.h"
#include "Render/RenderShadowSystem.h"
#include "Render/RenderTranslucentComponent.h"
#include "Render/RenderTranslucentSystem.h"
#include "Render/RenderUIPostComponent.h"
#include "Render/RenderUIPostSystem.h"
#include "Render/RenderUIPreComponent.h"
#include "Render/RenderUIPreSystem.h"
#include "Render/RenderVoxelSystem.h"

void render::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<render::CursorComponent>();
		entityWorld.RegisterComponent<render::DebugComponent>();
		entityWorld.RegisterComponent<render::ImGuiComponent>();
		entityWorld.RegisterComponent<render::OpaqueComponent>();
		entityWorld.RegisterComponent<render::ShadowComponent>();
		entityWorld.RegisterComponent<render::TranslucentComponent>();
		entityWorld.RegisterComponent<render::UIPostComponent>();
		entityWorld.RegisterComponent<render::UIPreComponent>();
	}

	// systems
	{
		entityWorld.RegisterSystem<render::CursorSystem>();
		entityWorld.RegisterSystem<render::DebugSystem>();
		entityWorld.RegisterSystem<render::ImGuiSystem>();
		entityWorld.RegisterSystem<render::OpaqueSystem>();
		entityWorld.RegisterSystem<render::ShadowSystem>();
		entityWorld.RegisterSystem<render::TranslucentSystem>();
		entityWorld.RegisterSystem<render::UIPostSystem>();
		entityWorld.RegisterSystem<render::UIPreSystem>();
		entityWorld.RegisterSystem<render::VoxelSystem>();
	}
}