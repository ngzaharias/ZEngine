#include "GameDebugPCH.h"
#include "GameDebug/SoftbodySystem.h"

#include "Core/Algorithms.h"
#include "Core/Array.h"
#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ColourHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Math/Algorithms.h"
#include "Math/Circle.h"
#include "Math/SplineMath.h"
#include "Softbody/SoftbodyComponents.h"

void dbg::SoftbodySystem::Update(World& world, const GameTime& gameTime)
{
	const auto& settings = world.ReadSingleton<const eng::settings::DebugComponent>();
	if (!settings.m_AreSoftbodyEnabled)
		return;

	auto& lines = world.WriteSingleton<eng::LinesComponent>();
	for (const ecs::Entity& chainEntity : world.Query<ecs::query::Include<const eng::TransformComponent, const softbody::ChainComponent>>())
	{
		const auto& chain = world.ReadComponent<softbody::ChainComponent>(chainEntity);
		for (const auto& [i, link] : enumerate::Forward(chain.m_Links))
		{
			const Colour& colour = colour::From(i);
			lines.AddCircle(link.m_Position.XY0(), Circle2f(10.f), 10.f, colour);
		}

		const int32 count = chain.m_Links.GetCount();
		for (int32 i = 1; i < count; ++i)
		{
			const Colour& colour = colour::From(i-1);
			const Vector2f& p0 = chain.m_Links[i - 1].m_Position;
			const Vector2f& p1 = chain.m_Links[i].m_Position;
			lines.AddLine(p0.XY0(), p1.XY0(), colour);
		}
	}
}
