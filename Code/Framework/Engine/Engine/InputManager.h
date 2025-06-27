#pragma once

#include "Core/Array.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Set.h"
#include "Input/Key.h"
#include "Input/Layer.h"
#include "Input/Value.h"
#include "Math/Vector.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class WindowManager;
}

namespace eng
{
	enum EInputPriority
	{
		Gameplay	= 0,
		MenuBar		= 1000,
		EditorWorld = 2000,
		GameUI		= 3000,
		Debug		= 4000,	// ImGui
		EditorUI	= 5000,
	};

	class InputManager final
	{
	public:
		using World = ecs::WorldView<
			const eng::WindowManager>;

		void Update(World& world);

		bool HasLayer(const str::Name& name) const;
		void AppendLayer(const str::Name& name, const input::Layer& layer);
		auto ModifyLayer(const str::Name& name)->input::Layer&;
		void RemoveLayer(const str::Name& name);

		auto GetLayers() const->const Array<str::Name>&;
		auto GetLayer(const str::Name& name) const->const input::Layer&;
		auto GetValue(const str::Name& name) const->const input::Value&;
		bool IsHeld(const str::Name& name) const;
		bool IsPressed(const str::Name& name) const;
		bool IsReleased(const str::Name& name) const;

		Vector2f m_MouseDelta = Vector2f::Zero;
		Vector2f m_MousePosition = Vector2f::Zero;
		Vector2f m_ScrollDelta = Vector2f::Zero;

	private:
		Set<input::EKey> m_Current;
		Set<input::EKey> m_Previous;

		bool m_AreLayersDirty = false;
		Array<str::Name> m_LayerOrder = {};
		Map<str::Name, int32> m_LayerRef = {};
		Map<str::Name, input::Layer> m_LayerMap = {};
		Map<str::Name, input::Value> m_ValueMap = {};
	};
}