#include "ClientCursorPCH.h"
#include "ClientCursor/CursorTransformSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/LinesComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/CollisionMath.h"
#include "Math/Plane.h"
#include "Math/Ray.h"
#include "Math/Sphere.h"
#include "Network/Peer.h"
#include "SharedCursor/CursorClientTransformEvent.h"
#include "SharedCursor/CursorTransformComponent.h"

namespace
{
	const str::Guid strSprite = GUID("9aacfb81dc064365d6ab5400264779a7");

	constexpr Vector3f s_Offset = Vector3f(9.f, -12.f, 0.f);
}

void client::cursor::TransformSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	using AddedQuery = ecs::query
		::Added<
		const shared::cursor::TransformComponent>
		::Include<
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<AddedQuery>())
	{
		const auto& peer = world.ReadResource<net::Peer>();
		const auto& serverComponent = view.ReadRequired<shared::cursor::TransformComponent>();
		if (serverComponent.m_PeerId == peer.GetPeerId())
			continue;

		world.AddComponent<ecs::NameComponent>(view, "Network Cursor");

		auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(view);
		spriteComponent.m_Sprite = strSprite;
		spriteComponent.m_Size = Vector2u(64, 64);

		auto& clientComponent = world.AddComponent<eng::TransformComponent>(view);
		clientComponent.m_Translate.x = serverComponent.m_Translate.x;
		clientComponent.m_Translate.y = serverComponent.m_Translate.y;
		clientComponent.m_Translate.z = -200.f;
	}

	using UpdateQuery = ecs::query
		::Updated<
		const shared::cursor::TransformComponent>
		::Include<
		const eng::TransformComponent,
		const shared::cursor::TransformComponent>;
	for (auto&& view : world.Query<UpdateQuery>())
	{
		const auto& serverComponent = view.ReadRequired<shared::cursor::TransformComponent>();
		auto& clientComponent = world.WriteComponent<eng::TransformComponent>(view);
		clientComponent.m_Translate.x = serverComponent.m_Translate.x + s_Offset.x;
		clientComponent.m_Translate.y = serverComponent.m_Translate.y + s_Offset.y;
	}

	using CameraQuery = ecs::query
		::Include<
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const eng::TransformComponent>;
	for (auto&& cameraView : world.Query<CameraQuery>())
	{
		const auto& peer = world.ReadResource<net::Peer>();
		if (!peer.IsConnected())
			continue;

		if (peer.GetPeerId().IsUnassigned())
			continue;

		const auto& input = world.ReadResource<eng::InputManager>();
		const auto& camera = cameraView.ReadRequired<eng::CameraComponent>();
		const auto& transform = cameraView.ReadRequired<eng::TransformComponent>();

		const Ray3f ray = eng::ScreenToRay(camera, transform, *window, input.m_MousePosition);
		const Plane3f plane = Plane3f(Vector3f::AxisZ);

		Vector3f worldPos;
		if (math::Intersection(ray, plane, worldPos))
		{
			auto& eventData = world.AddEvent<shared::cursor::ClientTransformEvent>();
			eventData.m_PeerId = peer.GetPeerId();
			eventData.m_Translate = worldPos;
		}
	}
}
