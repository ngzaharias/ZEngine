#include "GameUIPCH.h"
#include "GameUI/HiddenCountSystem.h"

#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "GameUI/DCHiddenCount.h"
#include "Hidden/HiddenCountComponent.h"

namespace
{
	const str::Name strHiddenCount_xaml = NAME("HiddenCount.xaml");
}

void gui::hidden::CountSystem::Update(World& world, const GameTime& gameTime)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Added<::hidden::CountComponent>>())
	{
		const auto& counter = world.ReadComponent<::hidden::CountComponent>(entity);
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strHiddenCount_xaml);
		auto& dataContext = uiManager.WriteDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);
		dataContext.SetObjectCount(counter.m_Revealed);
		dataContext.SetObjectTotal(counter.m_Objects);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Updated<::hidden::CountComponent>>())
	{
		const auto& counter = world.ReadComponent<::hidden::CountComponent>(entity);
		auto& uiManager = world.WriteResource<eng::UIManager>();
		auto& dataContext = uiManager.WriteDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);

		dataContext.SetObjectCount(counter.m_Revealed);
		dataContext.SetObjectTotal(counter.m_Objects);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<::hidden::CountComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strHiddenCount_xaml);
	}
}
