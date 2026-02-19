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
	struct AssetNewComponent;
	struct AssetOpenComponent;
	struct AssetSaveComponent;
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
			editor::sprite::AssetNewComponent,
			editor::sprite::AssetOpenComponent,
			editor::sprite::AssetSaveComponent,
			editor::sprite::BatchingComponent,
			editor::sprite::OpenWindowEvent,
			editor::sprite::SettingsSingleton,
			editor::sprite::WindowComponent,
			eng::AssetManager>
			::Read<
			eng::InputManager,
			gamestate::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
