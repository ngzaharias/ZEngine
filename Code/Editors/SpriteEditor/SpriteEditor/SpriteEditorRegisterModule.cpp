#include "SpriteEditorPCH.h"
#include "SpriteEditor/SpriteEditorRegisterModule.h"

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

void editor::sprite::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::sprite::AssetNewComponent>();
		world.RegisterComponent<editor::sprite::AssetOpenComponent>();
		world.RegisterComponent<editor::sprite::AssetSaveComponent>();
		world.RegisterComponent<editor::sprite::BatchingComponent>();
		world.RegisterComponent<editor::sprite::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::sprite::WindowRequest>();
	}

	// singleton
	{
		world.RegisterSingleton<editor::sprite::SettingsSingleton>();
	}

	// systems
	{
		world.RegisterSystem<editor::sprite::SettingsSystem>();
		world.RegisterSystem<editor::sprite::WindowSystem>();
	}
}