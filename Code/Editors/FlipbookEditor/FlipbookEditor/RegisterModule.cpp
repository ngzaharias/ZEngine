#include "FlipbookEditorPCH.h"
#include "FlipbookEditor/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "FlipbookEditor/FlipbookEditorAssetOpenComponent.h"
#include "FlipbookEditor/FlipbookEditorAssetSaveComponent.h"
#include "FlipbookEditor/FlipbookEditorBatchingComponent.h"
#include "FlipbookEditor/FlipbookEditorSettingsSingleton.h"
#include "FlipbookEditor/FlipbookEditorSettingsSystem.h"
#include "FlipbookEditor/FlipbookEditorWindowComponent.h"
#include "FlipbookEditor/FlipbookEditorWindowRequest.h"
#include "FlipbookEditor/FlipbookEditorWindowSystem.h"

void editor::flipbook::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<editor::flipbook::AssetOpenComponent>();
		world.RegisterComponent<editor::flipbook::AssetSaveComponent>();
		world.RegisterComponent<editor::flipbook::BatchingComponent>();
		world.RegisterComponent<editor::flipbook::WindowComponent>();
	}

	// events
	{
		world.RegisterEvent<editor::flipbook::WindowRequest>();
	}

	// singleton
	{
		world.RegisterSingleton<editor::flipbook::SettingsSingleton>();
	}

	// systems
	{
		world.RegisterSystem<editor::flipbook::SettingsSystem>();
		world.RegisterSystem<editor::flipbook::WindowSystem>();
	}
}