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

void editor::flipbook::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<editor::flipbook::AssetOpenComponent>();
		entityWorld.RegisterComponent<editor::flipbook::AssetSaveComponent>();
		entityWorld.RegisterComponent<editor::flipbook::BatchingComponent>();
		entityWorld.RegisterComponent<editor::flipbook::WindowComponent>();
	}

	// events
	{
		entityWorld.RegisterEvent<editor::flipbook::WindowRequest>();
	}

	// singleton
	{
		entityWorld.RegisterSingleton<editor::flipbook::SettingsSingleton>();
	}

	// systems
	{
		entityWorld.RegisterSystem<editor::flipbook::SettingsSystem>();
		entityWorld.RegisterSystem<editor::flipbook::WindowSystem>();
	}
}