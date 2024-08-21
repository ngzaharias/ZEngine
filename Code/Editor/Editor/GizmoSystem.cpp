#include "EditorPCH.h"
#include "Editor/GizmoSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LinesComponent.h"

void editor::GizmoSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// render coordinate lines
	if (false)
	{
		constexpr float s_Distance = 100000.f;
		constexpr Vector4f s_AxisX = Vector4f(1.f, 0.f, 0.f, 1.f);
		constexpr Vector4f s_AxisY = Vector4f(0.f, 1.f, 0.f, 1.f);
		constexpr Vector4f s_AxisZ = Vector4f(0.f, 0.f, 1.f, 1.f);

		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		linesComponent.AddLine(Vector3f::AxisX * -s_Distance, Vector3f::AxisX * s_Distance, s_AxisX);
		linesComponent.AddLine(Vector3f::AxisY * -s_Distance, Vector3f::AxisY * s_Distance, s_AxisY);
		linesComponent.AddLine(Vector3f::AxisZ * -s_Distance, Vector3f::AxisZ * s_Distance, s_AxisZ);
	}
}