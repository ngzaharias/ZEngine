#include "FlipbookEditorPCH.h"
#include "FlipbookEditor/FlipbookEditorRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "FlipbookEditor/FlipbookEditorAssetOpenComponent.h"
#include "FlipbookEditor/FlipbookEditorAssetSaveComponent.h"
#include "FlipbookEditor/FlipbookEditorBatchingComponent.h"
#include "FlipbookEditor/FlipbookEditorOpenWindowEvent.h"
#include "FlipbookEditor/FlipbookEditorSettingsSingleton.h"
#include "FlipbookEditor/FlipbookEditorSettingsSystem.h"
#include "FlipbookEditor/FlipbookEditorWindowComponent.h"
#include "FlipbookEditor/FlipbookEditorWindowSystem.h"

void editor::flipbook::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<editor::flipbook::AssetOpenComponent>();
	world.RegisterComponent<editor::flipbook::AssetSaveComponent>();
	world.RegisterComponent<editor::flipbook::BatchingComponent>();
	world.RegisterComponent<editor::flipbook::WindowComponent>();
	world.RegisterEvent<editor::flipbook::OpenWindowEvent>();
	world.RegisterSingleton<editor::flipbook::SettingsSingleton>();
	world.RegisterSystem<editor::flipbook::SettingsSystem>();
	world.RegisterSystem<editor::flipbook::WindowSystem>();
}