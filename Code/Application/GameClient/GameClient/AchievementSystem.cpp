#include "GameClientPCH.h"
#include "GameClient/AchievementSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AchievementTable.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PlatformManager.h"
#include "Engine/PrototypeComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Hidden/HiddenGroupComponent.h"
#include "Hidden/HiddenRevealComponent.h"
#include "Math/Matrix.h"

#include <PhysX/PxRigidActor.h>
#include <PhysX/PxScene.h>

namespace
{
	const str::Guid strAchievement_Clock = GUID("e9a27570dd6141ab910b1c54cca706d6");
	const str::Guid strAchievement_Globe = GUID("38f8294fbc664644a48a951a4595deb4");
	const str::Guid strAchievement_King = GUID("b985ad214fa04469aa89e87c62615e9e");
	const str::Guid strAchievement_Page = GUID("94784600eb634fd9b9576df2de735771");
	const str::Guid strObject_Clock = GUID("f3efea7d5c1242ede0b3669b044cb878");
	const str::Guid strObject_Globe = GUID("e6f344f0a03e46221b9eb9f32055d72d");
	const str::Guid strObject_King = GUID("bf3e41f80a6d49b2029bf1cc978d12af");

	const str::Name strInput = str::Name::Create("Achievement");
	const str::Name strSelect = str::Name::Create("Achievement_Select");

	Vector3f ToMouseDirection(const Vector3f& mousePosition, const eng::camera::ProjectionComponent& camera, const eng::TransformComponent& transform)
	{
		if (std::holds_alternative<eng::camera::Orthographic>(camera.m_Projection))
		{
			const Quaternion cameraRotate = Quaternion::FromRotator(transform.m_Rotate);
			return Vector3f::AxisZ * cameraRotate;
		}

		return (mousePosition - transform.m_Translate).Normalized();
	}
}

void client::AchievementSystem::Initialise(World& world)
{
	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::Gameplay;
	layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, false);

	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInput, layer);
}

void client::AchievementSystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void client::AchievementSystem::Update(World& world, const GameTime& gameTime)
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
			if (prototype.m_Guid == strObject_Globe)
			{
				const auto& achievements = world.ReadResource<eng::AchievementTable>();
				const auto& objectMap = achievements.GetObjectMap();
				if (objectMap.Contains(strAchievement_Globe))
				{
					const eng::Achievement& achievement = objectMap.Get(strAchievement_Globe);
					auto& platform = world.WriteResource<eng::PlatformManager>();
					platform.UnlockAchievement(achievement);
				}
			}
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Updated<hidden::GroupComponent>>())
	{
		const auto& groupComponent = world.ReadComponent<hidden::GroupComponent>(entity);
		if (groupComponent.m_Revealed.GetCount() != groupComponent.m_Objects.GetCount())
			continue;

		const auto& achievements = world.ReadResource<eng::AchievementTable>();
		const auto& objectMap = achievements.GetObjectMap();

		if (objectMap.Contains(strAchievement_Page))
		{
			const eng::Achievement& achievement = objectMap.Get(strAchievement_Page);
			auto& platform = world.WriteResource<eng::PlatformManager>();
			platform.UnlockAchievement(achievement);
		}

		if (groupComponent.m_Revealed.GetLast() == strObject_King)
		{
			if (objectMap.Contains(strAchievement_King))
			{
				const eng::Achievement& achievement = objectMap.Get(strAchievement_King);
				auto& platform = world.WriteResource<eng::PlatformManager>();
				platform.UnlockAchievement(achievement);
			}
		}
	}

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const auto& input = world.ReadResource<eng::InputManager>();
	const auto& physics = world.ReadSingleton<eng::PhysicsSceneComponent>();

	const Vector2u& resolution = window->GetResolution();
	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& camera = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Matrix4x4 cameraView = transform.ToTransform();

		// mouse
		constexpr float s_Distance = 100000.f;
		const Vector3f mousePosition = eng::camera::ScreenToWorld(input.m_MousePosition, camera.m_Projection, cameraView, resolution);
		const Vector3f mouseDirection = ToMouseDirection(mousePosition, camera, transform);

		const physx::PxVec3 position = {
			mousePosition.x,
			mousePosition.y,
			mousePosition.z };
		const physx::PxVec3 direction = {
			mouseDirection.x,
			mouseDirection.y,
			mouseDirection.z };

		physx::PxRaycastBuffer hitcall;
		physics.m_PhysicsScene->raycast(position, direction, s_Distance, hitcall);
		if (hitcall.hasAnyHits())
		{
			const physx::PxRaycastHit& closestHit = hitcall.block;
			const Vector3f hitPosition = Vector3f(closestHit.position.x, closestHit.position.y, closestHit.position.z);

			const ecs::Entity selectedEntity = reinterpret_cast<uint64>(closestHit.actor->userData);
			if (world.HasComponent<eng::PrototypeComponent>(selectedEntity))
			{
				const auto& prototype = world.ReadComponent<eng::PrototypeComponent>(selectedEntity);
				if (prototype.m_Guid == strObject_Clock && input.IsPressed(strSelect))
				{
					const auto& achievements = world.ReadResource<eng::AchievementTable>();
					const auto& objectMap = achievements.GetObjectMap();
					if (objectMap.Contains(strAchievement_Clock))
					{
						const eng::Achievement& achievement = objectMap.Get(strAchievement_Clock);
						auto& platform = world.WriteResource<eng::PlatformManager>();
						platform.UnlockAchievement(achievement);
					}
				}
			}
		}
	}
}