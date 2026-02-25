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

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	if (const eng::Window* window = windowManager.GetWindow(0))
	{
		const Vector2u& windowSize = window->GetSize();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowSize.x, windowSize.y);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_STENCIL_TEST);
		glClearStencil(0);
		glClearDepthf(1.f);

		// the depth mask must be enabled BEFORE clearing the depth buffer
		glDepthMask(GL_TRUE);
		glColorMask(true, true, true, true);
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}
