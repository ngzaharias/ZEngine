#include "EnginePCH.h"
#include "Engine/InputSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputComponent.h"
#include "Engine/GLFW/Window.h"

eng::InputSystem::InputSystem(glfw::Window& window)
	: m_Window(window)
{
}

void eng::InputSystem::Initialise(World& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, "Input");
	world.AddComponent<eng::InputComponent>(entity);
}

void eng::InputSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_KeyboardPrevious = std::move(m_KeyboardCurrent);
	m_MousePrevious = std::move(m_MouseCurrent);

	Vector2f glfwMouseDelta, glfwMousePos;
	m_Window.GatherKeyboard(m_KeyboardCurrent);
	m_Window.GatherMouse(m_MouseCurrent, glfwMouseDelta, glfwMousePos);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::InputComponent>>())
	{
		auto& component = world.GetComponent<eng::InputComponent>(entity);
		component.m_KeyboardPrevious = m_KeyboardPrevious;
		component.m_KeyboardCurrent = m_KeyboardCurrent;
		component.m_MousePrevious = m_MousePrevious;
		component.m_MouseCurrent = m_MouseCurrent;
		component.m_MouseDelta = glfwMouseDelta;
		component.m_MousePosition = glfwMousePos;
	}
}