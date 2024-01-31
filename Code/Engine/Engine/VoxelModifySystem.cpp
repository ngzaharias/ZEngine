#include "EnginePCH.h"
#include "Engine/VoxelModifySystem.h"

#include <Core/Algorithms.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/Screen.h"
#include "Engine/TransformComponent.h"
#include "Engine/VoxelComponents.h"

void voxel::ModifySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& linesComponent = world.GetSingleton<eng::LinesComponent>();
	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.GetComponent<const eng::CameraComponent>(cameraEntity);
		const auto& transformComponent = world.GetComponent<const eng::TransformComponent>(cameraEntity);

		const Vector3f direction = Vector3f::AxisZ * Quaternion::FromRotator(transformComponent.m_Rotate);
		const Vector3f worldPos = transformComponent.m_Translate + direction * 1000.f;
		const Vector3i gridPos = math::ToGridPos(worldPos, voxel::s_BlockSize1D);
		const Vector3f alignPos = math::ToWorldPos(gridPos, voxel::s_BlockSize1D);
		linesComponent.AddAABB(alignPos, voxel::s_BlockSize1D * 0.5f, Vector4f(1.f));

		for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& inputComponent = world.GetComponent<const eng::InputComponent>(inputEntity);
			if (inputComponent.IsKeyPressed(input::EKeyboard::F))
			{
				const ecs::Entity entity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<voxel::ModifyComponent>(entity);
				requestComponent.m_Changes.Emplace(alignPos, voxel::Block{ voxel::EFlags::None, voxel::EType::Black });
			}

			if (inputComponent.IsKeyPressed(input::EKeyboard::G))
			{
				const ecs::Entity entity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<voxel::ModifyComponent>(entity);
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, -voxel::s_BlockSize1D, -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , -voxel::s_BlockSize1D, -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, -voxel::s_BlockSize1D, -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, 0                    , -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , 0                    , -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, 0                    , -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, +voxel::s_BlockSize1D, -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , +voxel::s_BlockSize1D, -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, +voxel::s_BlockSize1D, -voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });

				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, -voxel::s_BlockSize1D, 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , -voxel::s_BlockSize1D, 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, -voxel::s_BlockSize1D, 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, 0                    , 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , 0                    , 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, 0                    , 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, +voxel::s_BlockSize1D, 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , +voxel::s_BlockSize1D, 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, +voxel::s_BlockSize1D, 0                    ), voxel::Block{ voxel::EFlags::None, voxel::EType::None });

				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, -voxel::s_BlockSize1D, +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , -voxel::s_BlockSize1D, +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, -voxel::s_BlockSize1D, +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, 0                    , +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , 0                    , +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, 0                    , +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(-voxel::s_BlockSize1D, +voxel::s_BlockSize1D, +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(0                    , +voxel::s_BlockSize1D, +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
				requestComponent.m_Changes.Emplace(alignPos + Vector3f(+voxel::s_BlockSize1D, +voxel::s_BlockSize1D, +voxel::s_BlockSize1D), voxel::Block{ voxel::EFlags::None, voxel::EType::None });
			}

			if (inputComponent.IsKeyHeld(input::EMouse::Left))
			{
				const ecs::Entity entity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<voxel::ModifyComponent>(entity);
				requestComponent.m_Changes.Emplace(alignPos, voxel::Block{ voxel::EFlags::None, voxel::EType::None });
			}
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const voxel::ModifyComponent>>())
		world.DestroyEntity(entity);
}
