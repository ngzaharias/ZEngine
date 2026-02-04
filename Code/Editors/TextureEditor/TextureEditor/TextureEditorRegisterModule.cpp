#include "TextureEditorPCH.h"
#include "TextureEditor/TextureEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "TextureEditor/TextureEditorAssetImportComponent.h"
#include "TextureEditor/TextureEditorAssetNewComponent.h"
#include "TextureEditor/TextureEditorAssetOpenComponent.h"
#include "TextureEditor/TextureEditorAssetSaveComponent.h"
#include "TextureEditor/TextureEditorSettingsSingleton.h"
#include "TextureEditor/TextureEditorSettingsSystem.h"
#include "TextureEditor/TextureEditorWindowComponent.h"
#include "TextureEditor/TextureEditorWindowRequest.h"
#include "TextureEditor/TextureEditorWindowSystem.h"

void editor::texture::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::texture::AssetImportComponent>();
		world.RegisterComponent<editor::texture::AssetNewComponent>();
		world.RegisterComponent<editor::texture::AssetOpenComponent>();
		world.RegisterComponent<editor::texture::AssetSaveComponent>();
		world.RegisterComponent<editor::texture::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::texture::WindowRequest>();
	}

	// singleton
	{
		world.RegisterSingleton<editor::texture::SettingsSingleton>();
	}

	// systems
	{
		world.RegisterSystem<editor::texture::WindowSystem>();
	}
}