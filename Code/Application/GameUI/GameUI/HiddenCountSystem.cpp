#include "GameUIPCH.h"
#include "GameUI/HiddenCountSystem.h"

#include "ClientHidden/HiddenCountComponent.h"
#include "Core/Name.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UIManager.h"
#include "GameUI/DCHiddenCount.h"

namespace
{
	const str::Name strHiddenCount_xaml = NAME("HiddenCount.xaml");
}

void gui::hidden::CountSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using AddedQuery = ecs::query
		::Added<client::hidden::CountComponent>
		::Include<const client::hidden::CountComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& counter = view.ReadRequired<client::hidden::CountComponent>();
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.CreateWidget(strHiddenCount_xaml);
		auto& dataContext = uiManager.WriteDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);
		dataContext.SetObjectCount(counter.m_Revealed);
		dataContext.SetObjectTotal(counter.m_Objects);
	}

	using UpdatedQuery = ecs::query
		::Updated<client::hidden::CountComponent>
		::Include<const client::hidden::CountComponent>;
	for (auto&& view : world.Query<UpdatedQuery>())
	{
		const auto& counter = view.ReadRequired<client::hidden::CountComponent>();
		auto& uiManager = world.WriteResource<eng::UIManager>();
		auto& dataContext = uiManager.WriteDataContext<gui::DCHiddenCount>(strHiddenCount_xaml);

		dataContext.SetObjectCount(counter.m_Revealed);
		dataContext.SetObjectTotal(counter.m_Objects);
	}

	for (auto&& view : world.Query<ecs::query::Removed<client::hidden::CountComponent>>())
	{
		auto& uiManager = world.WriteResource<eng::UIManager>();
		uiManager.DestroyWidget(strHiddenCount_xaml);
	}
}
