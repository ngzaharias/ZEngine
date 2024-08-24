#include "GameDebugPCH.h"
#include "GameDebug/HiddenObjectSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HiddenObjectComponents.h"
#include "Math/AABB.h"

namespace
{
	constexpr Vector4f s_ColourM = Vector4f(1.f, 0.f, 1.f, 1.f);
	constexpr Vector4f s_ColourW = Vector4f(1.f, 1.f, 1.f, 1.f);
}

void dbg::hidden::ObjectSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();

	using Query = ecs::query
		::Include<const ::hidden::ObjectComponent, const eng::TransformComponent>
		::Exclude<const ::hidden::RevealedComponent>;
	for (const ecs::Entity& entity : world.Query<Query>())
	{
		const auto& hiddenComponent = world.ReadComponent<::hidden::ObjectComponent>(entity);
		const auto& transformComponent = world.ReadComponent<eng::TransformComponent>(entity);

		const float sizeX = (float)hiddenComponent.m_Size.x * 0.5f;
		const float sizeY = (float)hiddenComponent.m_Size.y * 0.5f;
		const AABB3f s_Extents = AABB3f::FromExtents(Vector3f(sizeX, sizeY, 1.f));

		linesComponent.AddAABB(transformComponent.m_Translate, s_Extents, s_ColourM);
	}
}