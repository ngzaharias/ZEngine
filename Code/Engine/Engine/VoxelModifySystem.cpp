#include "EnginePCH.h"
#include "Engine/VoxelModifySystem.h"

#include <Core/AABB.h>
#include <Core/Algorithms.h>
#include <Core/CollisionMath.h>
#include <Core/Guid.h>
#include <Core/Raytracer.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/LinesComponent.h"
#include "Engine/Screen.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/VoxelComponents.h"

namespace
{
	int32 ToInnerIndex(const Vector3i& innerPos)
	{
		return innerPos.x
			+ innerPos.y * voxel::s_BlockCount1D
			+ innerPos.z * voxel::s_BlockCount2D;
	}

	Vector3f Raycast(voxel::ModifySystem::World& world, const Line& line)
	{
		Array<ecs::Entity> voxelEntities;
		auto& linesComponent = world.GetSingleton<eng::LinesComponent>();
		for (const ecs::Entity& voxelEntity : world.Query<ecs::query::Include<voxel::ChunkComponent, const eng::TransformComponent>>())
		{
			const auto& transform = world.GetComponent<const eng::TransformComponent>(voxelEntity);
			const AABB aabb = AABB(transform.m_Translate, transform.m_Translate + Vector3f(voxel::s_ChunkSize1D));

			Vector3f intersectPos;
			if (!math::Intersection(aabb, line, intersectPos))
				continue;

			linesComponent.AddAABB(
				transform.m_Translate + Vector3f(voxel::s_ChunkSize1D * 0.5f), 
				voxel::s_ChunkSize1D * 0.51f, 
				Vector4f(1.f));
			voxelEntities.Append(voxelEntity);
		}

		const path::Raytracer raytracer(line.m_PointA, line.m_PointB, voxel::s_BlockSize1D);
		for (const Vector3i& gridPos : raytracer)
		{
			const Vector3f worldPos = math::ToWorldPos(gridPos, voxel::s_BlockSize1D);
			for (const ecs::Entity& voxelEntity : voxelEntities)
			{
				const auto& transform = world.GetComponent<const eng::TransformComponent>(voxelEntity);

				const Vector3i requestPos = math::ToGridPos(worldPos - transform.m_Translate, voxel::s_ChunkSize1D);
				if (requestPos != Vector3i::Zero)
					continue;

				const Vector3f localPos = worldPos - transform.m_Translate;
				const Vector3i innerPos = math::ToGridPos(localPos, voxel::s_BlockSize1D);
				const int32 innerIndex = ToInnerIndex(innerPos);
				const auto& chunkComponent = world.GetComponent<const voxel::ChunkComponent>(voxelEntity);
				const voxel::Block& block = chunkComponent.m_Data[innerIndex];
				if (block.m_Type == voxel::EType::None)
					continue;

				return worldPos;
			}
		}
		return line.m_PointB;
	}
}

void voxel::ModifySystem::Initialise(World& world)
{
	world.AddSingleton<voxel::ModifySettingsComponent>();
	auto& transformComponent = world.AddComponent<eng::TransformComponent>(ecs::Entity(0));
	transformComponent.m_Translate.z = -100;
	auto& textComponent = world.AddComponent<eng::TextComponent>(ecs::Entity(0));
	textComponent.m_Font = str::Guid::Create("186be450-6236-4741-977b-c2b0568f5368");
}

void voxel::ModifySystem::Shutdown(World& world)
{
	world.RemoveSingleton<voxel::ModifySettingsComponent>();
}

void voxel::ModifySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& linesComponent = world.GetSingleton<eng::LinesComponent>();
	auto& settingsComponent = world.GetSingleton<voxel::ModifySettingsComponent>();
	auto& textComponent = world.GetComponent<eng::TextComponent>(ecs::Entity(0));
	textComponent.m_Text = std::format(
		"Type = {}, Radius = {}", 
		(int)settingsComponent.m_Type, 
		settingsComponent.m_Radius);

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.GetComponent<const eng::CameraComponent>(cameraEntity);
		const auto& transformComponent = world.GetComponent<const eng::TransformComponent>(cameraEntity);

		const Line line = Line(
			transformComponent.m_Translate, 
			transformComponent.m_Translate + 
			Vector3f::AxisZ * Quaternion::FromRotator(transformComponent.m_Rotate) * 
			100000.f);
		const Vector3f worldPos = Raycast(world, line);
		const Vector3i gridPos = math::ToGridPos(worldPos, voxel::s_BlockSize1D);
		const Vector3f alignPos = math::ToWorldPos(gridPos, voxel::s_BlockSize1D);
		linesComponent.AddAABB(alignPos, voxel::s_BlockSize1D * 0.5f, Vector4f(1.f));

		for (const ecs::Entity& inputEntity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& inputComponent = world.GetComponent<const eng::InputComponent>(inputEntity);
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_1))
				settingsComponent.m_Radius = 1;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_2))
				settingsComponent.m_Radius = 2;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_3))
				settingsComponent.m_Radius = 3;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_4))
				settingsComponent.m_Radius = 4;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_5))
				settingsComponent.m_Radius = 5;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_6))
				settingsComponent.m_Radius = 6;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Numpad_0))
				settingsComponent.m_Radius = 0;

			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_1))
				settingsComponent.m_Type = voxel::EType::Black;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_2))
				settingsComponent.m_Type = voxel::EType::Green;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_3))
				settingsComponent.m_Type = voxel::EType::Grey;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_4))
				settingsComponent.m_Type = voxel::EType::Orange;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_5))
				settingsComponent.m_Type = voxel::EType::Purple;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_6))
				settingsComponent.m_Type = voxel::EType::Red;
			if (inputComponent.IsKeyPressed(input::EKeyboard::Num_0))
				settingsComponent.m_Type = voxel::EType::None;

			if (inputComponent.IsKeyPressed(input::EMouse::Left))
			{
				const ecs::Entity entity = world.CreateEntity();
				auto& requestComponent = world.AddComponent<voxel::ModifyComponent>(entity);

				const int32 radius = settingsComponent.m_Radius;
				const voxel::EType type = settingsComponent.m_Type;
				for (const Vector3i& index : enumerate::Vector(Vector3i(-radius), Vector3i(+radius)))
				{
					const Vector3f offset = Vector3f(
						voxel::s_BlockSize1D * index.x,
						voxel::s_BlockSize1D * index.y,
						voxel::s_BlockSize1D * index.z);

					requestComponent.m_Changes.Emplace(alignPos + offset, voxel::Block{ voxel::EFlags::None, type });
				}
			}
		}
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const voxel::ModifyComponent>>())
		world.DestroyEntity(entity);
}
