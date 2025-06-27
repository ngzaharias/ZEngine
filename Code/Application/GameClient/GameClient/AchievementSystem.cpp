#include "GameClientPCH.h"
#include "GameClient/AchievementSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AchievementTable.h"
#include "Engine/PlatformManager.h"
#include "Engine/PrototypeComponent.h"

namespace
{
	const str::Guid strAchievement = str::Guid::Create("38f8294fbc664644a48a951a4595deb4");
	const str::Guid strHiddenObject = str::Guid::Create("e6f344f0a03e46221b9eb9f32055d72d");
}

void clt::AchievementSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// hidden object revealed
	{
		using Query = ecs::query
			::Added<const hidden::RevealComponent>
			::Include<const eng::PrototypeComponent>;

		for (const ecs::Entity& objectEntity : world.Query<Query>())
		{
			const auto& prototype = world.ReadComponent<eng::PrototypeComponent>(objectEntity);
			if (prototype.m_Guid == strHiddenObject)
			{
				const auto& achievements = world.ReadResource<eng::AchievementTable>();
				const auto& objectMap = achievements.GetObjectMap();
				if (objectMap.Contains(strAchievement))
				{
					const eng::Achievement& achievement = objectMap.Get(strAchievement);
					auto& platform = world.WriteResource<eng::PlatformManager>();
					platform.UnlockAchievement(achievement);
				}
			}
		}
	}
}