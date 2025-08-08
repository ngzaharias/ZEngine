#include "EnginePCH.h"
#include "Engine/RenderStage_Noesis_Pre.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	constexpr Colour s_ClearColour = Colour(0.24f);
}

void eng::RenderStage_Noesis_Pre::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	auto& uiManager = world.WriteResource<eng::UIManager>();
	uiManager.RenderBegin();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	if (const eng::Window* window = windowManager.GetWindow(0))
	{
		const Vector2u& resolution = window->GetResolution();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, resolution.x, resolution.y);
		glDisable(GL_SCISSOR_TEST);
		glClearStencil(0);
		glClearDepthf(1.f);
		glClearColor(s_ClearColour.r, s_ClearColour.g, s_ClearColour.b, 0.f);

		// the depth mask must be enabled BEFORE clearing the depth buffer
		glDepthMask(GL_TRUE);
		glColorMask(true, true, true, true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}
