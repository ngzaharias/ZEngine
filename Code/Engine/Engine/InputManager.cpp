#include "EnginePCH.h"
#include "Engine/InputManager.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Input/Binding.h"

// #todo: remove
#include "imgui/imgui.h"

namespace
{
	constexpr input::Value s_Empty = {};
	const str::Guid s_ImGui = str::Guid::Generate();
}

void eng::InputManager::Initialise()
{
	PROFILE_FUNCTION();

	// #todo: move to a system
	{
		AppendLayer(s_ImGui, input::Layer{});
	}
}

void eng::InputManager::Shutdown()
{
	PROFILE_FUNCTION();

	RemoveLayer(s_ImGui);
}

void eng::InputManager::Update(World& world)
{
	PROFILE_FUNCTION();

	// #todo: move to a system
	{
		input::Layer& layer = ModifyLayer(s_ImGui);
		const bool hasKeyboard = layer.m_Consume.Has(input::EConsume::Keyboard);
		const bool hasMouse = layer.m_Consume.Has(input::EConsume::Mouse);
		const bool wantsKeyboard = ImGui::GetIO().WantCaptureKeyboard;
		const bool wantsMouse = ImGui::GetIO().WantCaptureMouse;
		if (hasKeyboard != wantsKeyboard || hasMouse != wantsMouse)
		{
			layer.m_Consume.Set(input::EConsume::Keyboard, wantsKeyboard);
			layer.m_Consume.Set(input::EConsume::Mouse, wantsMouse);
		}
	}

	if (m_AreLayersDirty)
	{
		m_AreLayersDirty = false;

		m_LayerOrder.RemoveAll();
		for (const auto& [guid, layer] : m_LayerMap)
			m_LayerOrder.Append(guid);

		// sort them so highest priority is first
		std::sort(m_LayerOrder.begin(), m_LayerOrder.end(),
			[&](const str::Guid& lhs, const str::Guid& rhs)
			{
				const input::Layer& a = m_LayerMap[lhs];
				const input::Layer& b = m_LayerMap[rhs];
				return a.m_Priority > b.m_Priority;
			});
	}

	const auto& manager = world.ReadResource<eng::WindowManager>();
	if (const eng::Window* window = manager.GetWindow(0))
	{
		std::swap(m_CachePrev.m_Keyboard, m_CacheCurr.m_Keyboard);
		std::swap(m_CacheCurr.m_Mouse,    m_CacheCurr.m_Mouse);
		m_CacheCurr.m_Keyboard.RemoveAll();
		m_CacheCurr.m_Mouse.RemoveAll();
		m_ValueMap.RemoveAll();

		Vector2f mouseDelta, mousePos, scrollDelta;
		window->GatherKeyboard(m_CacheCurr.m_Keyboard);
		window->GatherMouse(m_CacheCurr.m_Mouse, mouseDelta, mousePos);
		window->GatherScroll(scrollDelta);

		Set<input::EKeyboard> pressedK, releasedK;
		Set<input::EMouse> pressedM, releasedM;
		enumerate::Difference(m_CacheCurr.m_Keyboard, m_CachePrev.m_Keyboard, pressedK);
		enumerate::Difference(m_CachePrev.m_Keyboard, m_CacheCurr.m_Keyboard, releasedK);

		Set<input::EKeyboard> heldK = m_CacheCurr.m_Keyboard;
		Set<input::EMouse> heldM = m_CacheCurr.m_Mouse;
		for (const str::Guid& guid : m_LayerOrder)
		{
			const input::Layer& layer = m_LayerMap[guid];
			for (const input::Binding& binding : layer.m_Bindings)
			{
				if (std::holds_alternative<input::EKeyboard>(binding.m_Key))
				{
					const auto key = std::get<input::EKeyboard>(binding.m_Key);
					if (heldK.Contains(key))
					{
						m_ValueMap[binding.m_Name].m_Value = 1.f;
						if (binding.m_Consume)
							heldK.Remove(key);
					}

					if (pressedK.Contains(key))
					{
						m_ValueMap[binding.m_Name].m_Pressed = true;
						if (binding.m_Consume)
							pressedK.Remove(key);
					}

					if (releasedK.Contains(key))
					{
						m_ValueMap[binding.m_Name].m_Released = true;
						if (binding.m_Consume)
							releasedK.Remove(key);
					}
				}

				if (std::holds_alternative<input::EMouse>(binding.m_Key))
				{
					const auto key = std::get<input::EMouse>(binding.m_Key);
					if (heldM.Contains(key))
					{
						m_ValueMap[binding.m_Name].m_Value = 1.f;
						if (binding.m_Consume)
							heldM.Remove(key);
					}

					if (pressedM.Contains(key))
					{
						m_ValueMap[binding.m_Name].m_Pressed = true;
						if (binding.m_Consume)
							pressedM.Remove(key);
					}

					if (releasedM.Contains(key))
					{
						m_ValueMap[binding.m_Name].m_Released = true;
						if (binding.m_Consume)
							releasedM.Remove(key);
					}
				}
			}

			if (layer.m_Consume.Has(input::EConsume::Keyboard))
			{
				heldK.RemoveAll();
				pressedK.RemoveAll();
				releasedK.RemoveAll();
			}

			if (layer.m_Consume.Has(input::EConsume::Mouse))
			{
				heldM.RemoveAll();
				pressedM.RemoveAll();
				releasedM.RemoveAll();
			}
		}
	}
}

void eng::InputManager::AppendLayer(const str::Guid& guid, const input::Layer& layer)
{
	m_AreLayersDirty = true;
	m_LayerMap[guid] = layer;
}

auto eng::InputManager::ModifyLayer(const str::Guid& guid)->input::Layer&
{
	return m_LayerMap.Get(guid);
}

void eng::InputManager::RemoveLayer(const str::Guid& guid)
{
	m_AreLayersDirty = true;
	m_LayerMap.Remove(guid);
}

auto eng::InputManager::GetValue(const str::Name& name) const->const input::Value&
{
	auto find = m_ValueMap.Find(name);
	if (find != m_ValueMap.end())
		return find->second;

	return s_Empty;
}

bool eng::InputManager::IsHeld(const str::Name& name) const
{
	return m_ValueMap.Contains(name);
}

bool eng::InputManager::IsPressed(const str::Name& name) const
{
	auto find = m_ValueMap.Find(name);
	if (find != m_ValueMap.end())
		return find->second.m_Pressed;

	return false;
}

bool eng::InputManager::IsReleased(const str::Name& name) const
{
	auto find = m_ValueMap.Find(name);
	if (find != m_ValueMap.end())
		return find->second.m_Released;

	return false;
}