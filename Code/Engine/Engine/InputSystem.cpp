#include "EnginePCH.h"
#include "Engine/InputSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include "imgui/imgui.h"

void eng::InputSystem::Initialise(World& world)
{
	const ecs::Entity entity = world.CreateEntity();
	world.AddComponent<ecs::NameComponent>(entity, "Input");
	world.AddComponent<eng::InputComponent>(entity);
}

void eng::InputSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& manager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = manager.GetWindow(0);

	std::swap(m_KeyboardPrevious, m_KeyboardCurrent);
	std::swap(m_MousePrevious, m_MouseCurrent);
	m_KeyboardCurrent.RemoveAll();
	m_MouseCurrent.RemoveAll();

	Vector2f mouseDelta, mousePos, scrollDelta;
	window->GatherKeyboard(m_KeyboardCurrent);
	window->GatherMouse(m_MouseCurrent, mouseDelta, mousePos);
	window->GatherScroll(scrollDelta);

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<eng::InputComponent>>())
	{
		auto& component = world.WriteComponent<eng::InputComponent>(entity);
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			component.m_KeyboardPrevious = m_KeyboardPrevious;
			component.m_KeyboardCurrent = m_KeyboardCurrent;
		}

		if (!ImGui::GetIO().WantCaptureMouse)
		{
			component.m_MousePrevious = m_MousePrevious;
			component.m_MouseCurrent = m_MouseCurrent;
			component.m_MouseDelta = mouseDelta;
			component.m_MousePosition = mousePos;
			component.m_ScrollDelta = scrollDelta;
		}
	}
}