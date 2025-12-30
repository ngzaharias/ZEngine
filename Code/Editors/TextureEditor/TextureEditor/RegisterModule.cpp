#include "TextureEditorPCH.h"
#include "TextureEditor/RegisterModule.h"

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

void editor::texture::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<editor::texture::AssetImportComponent>();
		entityWorld.RegisterComponent<editor::texture::AssetNewComponent>();
		entityWorld.RegisterComponent<editor::texture::AssetOpenComponent>();
		entityWorld.RegisterComponent<editor::texture::AssetSaveComponent>();
		entityWorld.RegisterComponent<editor::texture::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<editor::texture::WindowRequest>();
	}

	// singleton
	{
		entityWorld.RegisterSingleton<editor::texture::SettingsSingleton>();
	}

	// systems
	{
		entityWorld.RegisterSystem<editor::texture::WindowSystem>();
	}
}