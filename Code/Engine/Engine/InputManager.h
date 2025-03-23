#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Set.h"
#include "Input/Keyboard.h"
#include "Input/Layer.h"
#include "Input/Mouse.h"
#include "Input/Value.h"

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
		Gameplay = 0,
		Debug = 100,
		GameUI = 200,
		Editor = 300,
	};

	class InputManager final
	{
		struct Cache
		{
			Set<input::EKeyboard> m_Keyboard;
			Set<input::EMouse>    m_Mouse;
		};

	public:
		using World = ecs::WorldView<
			const eng::WindowManager>;

		void Initialise();
		void Shutdown();

		void Update(World& world);

		void AppendLayer(const str::Guid& guid, const input::Layer& layer);
		auto ModifyLayer(const str::Guid& guid)->input::Layer&;
		void RemoveLayer(const str::Guid& guid);

		auto GetValue(const str::Name& name) const->const input::Value&;
		bool IsHeld(const str::Name& name) const;
		bool IsPressed(const str::Name& name) const;
		bool IsReleased(const str::Name& name) const;

	private:
		Cache m_CacheCurr = {};
		Cache m_CachePrev = {};

		bool m_AreLayersDirty = false;
		Array<str::Guid> m_LayerOrder = {};
		Map<str::Guid, input::Layer> m_LayerMap = {};
		Map<str::Name, input::Value> m_ValueMap = {};
	};
}