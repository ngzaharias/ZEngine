#include "RenderPCH.h"
#include "Render/RenderUIPostSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

void render::UIPostSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& uiManager = world.WriteResource<eng::UIManager>();
	uiManager.RenderFinish();

	glColorMask(true, true, true, true);
}
