#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenSaveLoadSystem.h"

#include "ClientHidden/HiddenGroupComponent.h"
#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "Core/Algorithms.h"
#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SavegameComponent.h"
#include "Engine/UUIDComponent.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strFilename = "Savegame.toml";
	const str::StringView strRevealed = "m_Revealed";
}

void client::hidden::SaveLoadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<ecs::query::Updated<const client::hidden::GroupComponent>>())
	{
		Visitor visitor;
		for (auto&& view : world.Query<ecs::query::Include<const client::hidden::GroupComponent>>())
		{
			const auto& component = view.ReadRequired<client::hidden::GroupComponent>();
			visitor.Write(strRevealed, component.m_Revealed);
		}

		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);
		visitor.SaveToFile(filepath);
	}

	using Query = ecs::query
		::Added<const client::hidden::ObjectComponent>
		::Include<const eng::UUIDComponent>
		::Exclude<const client::hidden::RevealComponent>;
	if (world.HasAny<Query>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		Visitor visitor;
		visitor.LoadFromFile(filepath);

		Array<str::Guid> revealed;
		visitor.Read(strRevealed, revealed, {});

		for (auto&& view : world.Query<Query>())
		{
			const auto& uuidComponent = view.ReadRequired<eng::UUIDComponent>();
			if (enumerate::Contains(revealed, uuidComponent.m_UUID))
			{
				world.AddComponent<eng::SavegameComponent>(view);
				world.AddComponent<client::hidden::RevealComponent>(view);
			}
		}
	}
}