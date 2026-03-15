#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::sprite
{
	struct AssetNewEvent;
	struct AssetOpenEvent;
	struct AssetSaveEvent;
	struct BatchingComponent;
	struct OpenWindowEvent;
	struct SettingsSingleton;
	struct WindowComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
}

namespace gamestate
{
	struct EditorComponent;
}

namespace editor::sprite
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::sprite::AssetNewEvent,
			editor::sprite::AssetOpenEvent,
			editor::sprite::AssetSaveEvent,
			editor::sprite::BatchingComponent,
			editor::sprite::OpenWindowEvent,
			editor::sprite::SettingsSingleton,
			editor::sprite::WindowComponent,
			eng::AssetManager,
			eng::InputManager>
			::Read<
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
