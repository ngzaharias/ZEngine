#include "EnginePCH.h"
#include "Engine/InputManager.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Input/Binding.h"

namespace
{
	constexpr input::Value s_Empty = {};
}

void eng::InputManager::Update(World& world)
{
	PROFILE_FUNCTION();

	if (m_AreLayersDirty)
	{
		m_AreLayersDirty = false;

		m_LayerOrder.RemoveAll();
		for (const auto& [guid, layer] : m_LayerMap)
			m_LayerOrder.Append(guid);

		// sort them so highest priority is first
		std::sort(m_LayerOrder.begin(), m_LayerOrder.end(),
			[&](const str::Name& lhs, const str::Name& rhs)
			{
				const input::Layer& a = m_LayerMap[lhs];
				const input::Layer& b = m_LayerMap[rhs];
				return a.m_Priority > b.m_Priority;
			});
	}

	const auto& manager = world.ReadResource<eng::WindowManager>();
	if (const eng::Window* window = manager.GetWindow(0))
	{
		std::swap(m_Previous, m_Current);
		m_Current.RemoveAll();
		m_ValueMap.RemoveAll();

		window->GatherGamepad(m_Current);
		window->GatherKeyboard(m_Current);
		window->GatherMouse(m_Current, m_MouseDelta, m_MousePosition);
		window->GatherScroll(m_ScrollDelta);

		Set<input::EKey> held = m_Current;
		Set<input::EKey> pressed, released;
		enumerate::Difference(m_Current, m_Previous, pressed);
		enumerate::Difference(m_Previous, m_Current, released);

		for (const str::Name& name : m_LayerOrder)
		{
			const input::Layer& layer = m_LayerMap[name];
			layer.m_Callback.Publish(m_MousePosition, m_MouseDelta, m_ScrollDelta, held, pressed, released);

			for (const input::Binding& binding : layer.m_Bindings)
			{
				bool modifier = binding.m_Modifier == input::EKey::None;
				if (held.Contains(binding.m_Modifier))
					modifier = true;

				if (modifier && held.Contains(binding.m_Primary))
				{
					m_ValueMap[binding.m_Name].m_Value = 1.f;
					if (binding.m_Consume)
						held.Remove(binding.m_Primary);
				}

				if (modifier && pressed.Contains(binding.m_Primary))
				{
					m_ValueMap[binding.m_Name].m_Pressed = true;
					if (binding.m_Consume)
						pressed.Remove(binding.m_Primary);
				}

				if (modifier && released.Contains(binding.m_Primary))
				{
					m_ValueMap[binding.m_Name].m_Released = true;
					if (binding.m_Consume)
						released.Remove(binding.m_Primary);
				}
			}

			if (layer.m_Consume.Has(input::EConsume::Keyboard))
			{
				held.RemoveAll();
				pressed.RemoveAll();
				released.RemoveAll();
			}

			if (layer.m_Consume.Has(input::EConsume::Mouse))
			{
				held.RemoveAll();
				pressed.RemoveAll();
				released.RemoveAll();
				m_ScrollDelta = Vector2f::Zero;
			}
		}
	}
}

bool eng::InputManager::HasLayer(const str::Name& name) const
{
	return m_LayerRef.Contains(name);
}

void eng::InputManager::AppendLayer(const str::Name& name, const input::Layer& layer)
{
	int32& refCount = m_LayerRef[name];
	if (++refCount == 1)
	{
		m_AreLayersDirty = true;
		m_LayerMap[name] = layer;
	}
}

auto eng::InputManager::ModifyLayer(const str::Name& name)->input::Layer&
{
	return m_LayerMap.Get(name);
}

void eng::InputManager::RemoveLayer(const str::Name& name)
{
	int32& refCount = m_LayerRef[name];
	if (--refCount == 0)
	{
		m_AreLayersDirty = true;
		m_LayerRef.Remove(name);
		m_LayerMap.Remove(name);
	}
}

auto eng::InputManager::GetLayers() const->const Array<str::Name>&
{
	return m_LayerOrder;
}

auto eng::InputManager::GetLayer(const str::Name& name) const->const input::Layer&
{
	auto find = m_LayerMap.Find(name);
	if (find != m_LayerMap.end())
		return find->second;

	static input::Layer layer;
	return layer;
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