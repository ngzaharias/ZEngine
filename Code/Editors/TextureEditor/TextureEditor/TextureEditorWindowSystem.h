#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::texture
{
	struct AssetImportComponent;
	struct AssetNewComponent;
	struct AssetOpenComponent;
	struct AssetSaveComponent;
	struct OpenWindowEvent;
	struct SettingsSingleton;
	struct WindowComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
}

namespace editor::texture
{
	class WindowSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			ecs::NameComponent,
			editor::texture::AssetImportComponent,
			editor::texture::AssetNewComponent,
			editor::texture::AssetOpenComponent,
			editor::texture::AssetSaveComponent,
			editor::texture::SettingsSingleton,
			editor::texture::WindowComponent,
			eng::AssetManager>
			::Read<
			editor::texture::OpenWindowEvent,
			eng::InputManager>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}
