#include "SpriteEditorPCH.h"
#include "SpriteEditor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "SpriteEditor/SpriteEditorAssetNewComponent.h"
#include "SpriteEditor/SpriteEditorAssetOpenComponent.h"
#include "SpriteEditor/SpriteEditorAssetSaveComponent.h"
#include "SpriteEditor/SpriteEditorBatchingComponent.h"
#include "SpriteEditor/SpriteEditorSettingsSingleton.h"
#include "SpriteEditor/SpriteEditorSettingsSystem.h"
#include "SpriteEditor/SpriteEditorWindowComponent.h"
#include "SpriteEditor/SpriteEditorWindowRequest.h"
#include "SpriteEditor/SpriteEditorWindowSystem.h"

void editor::sprite::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<editor::sprite::AssetNewComponent>();
		entityWorld.RegisterComponent<editor::sprite::AssetOpenComponent>();
		entityWorld.RegisterComponent<editor::sprite::AssetSaveComponent>();
		entityWorld.RegisterComponent<editor::sprite::BatchingComponent>();
		entityWorld.RegisterComponent<editor::sprite::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<editor::sprite::WindowRequest>();
	}

	// singleton
	{
		entityWorld.RegisterSingleton<editor::sprite::SettingsSingleton>();
	}

	// systems
	{
		entityWorld.RegisterSystem<editor::sprite::SettingsSystem>();
		entityWorld.RegisterSystem<editor::sprite::WindowSystem>();
	}
}