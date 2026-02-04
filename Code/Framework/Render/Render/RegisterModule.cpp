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

void render::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<render::CursorComponent>();
		world.RegisterComponent<render::DebugComponent>();
		world.RegisterComponent<render::ImGuiComponent>();
		world.RegisterComponent<render::OpaqueComponent>();
		world.RegisterComponent<render::ShadowComponent>();
		world.RegisterComponent<render::TranslucentComponent>();
		world.RegisterComponent<render::UIPostComponent>();
		world.RegisterComponent<render::UIPreComponent>();
	}

	// systems
	{
		world.RegisterSystem<render::CursorSystem>();
		world.RegisterSystem<render::DebugSystem>();
		world.RegisterSystem<render::ImGuiSystem>();
		world.RegisterSystem<render::OpaqueSystem>();
		world.RegisterSystem<render::ShadowSystem>();
		world.RegisterSystem<render::TranslucentSystem>();
		world.RegisterSystem<render::UIPostSystem>();
		world.RegisterSystem<render::UIPreSystem>();
		world.RegisterSystem<render::VoxelSystem>();
	}
}