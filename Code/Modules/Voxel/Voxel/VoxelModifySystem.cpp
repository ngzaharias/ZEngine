#include "VoxelPCH.h"
#include "Voxel/VoxelModifySystem.h"

#include "Core/Algorithms.h"
#include "Core/Guid.h"
#include "Core/Raytracer.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/LinesComponent.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Voxel/VoxelComponents.h"
#include "Math/AABB.h"
#include "Math/Algorithms.h"
#include "Math/CollisionMath.h"

// #todo: enable only when voxels are present

namespace
{
	const str::Name strInput = str::Name::Create("VoxelModify");
	const str::Name strRadius0 = str::Name::Create("VoxelModify_Radius0");
	const str::Name strRadius1 = str::Name::Create("VoxelModify_Radius1");
	const str::Name strRadius2 = str::Name::Create("VoxelModify_Radius2");
	const str::Name strRadius3 = str::Name::Create("VoxelModify_Radius3");
	const str::Name strRadius4 = str::Name::Create("VoxelModify_Radius4");
	const str::Name strRadius5 = str::Name::Create("VoxelModify_Radius5");
	const str::Name strRadius6 = str::Name::Create("VoxelModify_Radius6");
	const str::Name strSelect = str::Name::Create("VoxelModify_Select");
	const str::Name strVoxel0 = str::Name::Create("VoxelModify_Voxel0");
	const str::Name strVoxel1 = str::Name::Create("VoxelModify_Voxel1");
	const str::Name strVoxel2 = str::Name::Create("VoxelModify_Voxel2");
	const str::Name strVoxel3 = str::Name::Create("VoxelModify_Voxel3");
	const str::Name strVoxel4 = str::Name::Create("VoxelModify_Voxel4");
	const str::Name strVoxel5 = str::Name::Create("VoxelModify_Voxel5");
	const str::Name strVoxel6 = str::Name::Create("VoxelModify_Voxel6");

	int32 ToInnerIndex(const Vector3i& innerPos)
	{
		return innerPos.x
			+ innerPos.y * voxel::s_BlockCount1D
			+ innerPos.z * voxel::s_BlockCount2D;
	}

	Vector3f Raycast(voxel::ModifySystem::World& world, const Segment3f& segment)
	{
		Array<ecs::Entity> voxelEntities;
		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		for (const ecs::Entity& voxelEntity : world.Query<ecs::query::Include<voxel::ChunkComponent, const eng::TransformComponent>>())
		{
			const auto& transform = world.ReadComponent<eng::TransformComponent>(voxelEntity);
			const AABB3f aabb = AABB3f(transform.m_Translate, transform.m_Translate + Vector3f(voxel::s_ChunkSize1D));

			Vector3f intersectPos;
			if (!math::Intersection(aabb, segment, intersectPos))
				continue;

			linesComponent.AddAABB(
				transform.m_Translate + Vector3f(voxel::s_ChunkSize1D * 0.5f),
				voxel::s_ChunkSize1D * 0.51f,
				Vector4f(1.f));
			voxelEntities.Append(voxelEntity);
		}

		const path::Raytracer raytracer(segment.m_PointA, segment.m_PointB, voxel::s_BlockSize1D);
		for (const Vector3i& gridPos : raytracer)
		{
			const Vector3f worldPos = math::ToWorldPos(gridPos, voxel::s_BlockSize1D);
			for (const ecs::Entity& voxelEntity : voxelEntities)
			{
				const auto& transform = world.ReadComponent<eng::TransformComponent>(voxelEntity);

				const Vector3i requestPos = math::ToGridPos(worldPos - transform.m_Translate, voxel::s_ChunkSize1D);
				if (requestPos != Vector3i::Zero)
					continue;

				const Vector3f localPos = worldPos - transform.m_Translate;
				const Vector3i innerPos = math::ToGridPos(localPos, voxel::s_BlockSize1D);
				const int32 innerIndex = ToInnerIndex(innerPos);
				const auto& chunkComponent = world.ReadComponent<voxel::ChunkComponent>(voxelEntity);
				const voxel::Block& block = chunkComponent.m_Data[innerIndex];
				if (block.m_Type == voxel::EType::None)
					continue;

				return worldPos;
			}
		}
		return segment.m_PointB;
	}
}

void voxel::ModifySystem::Initialise(World& world)
{
	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::Gameplay;
	layer.m_Bindings.Emplace(strRadius0, input::EKey::Numpad_0);
	layer.m_Bindings.Emplace(strRadius1, input::EKey::Numpad_1);
	layer.m_Bindings.Emplace(strRadius2, input::EKey::Numpad_2);
	layer.m_Bindings.Emplace(strRadius3, input::EKey::Numpad_3);
	layer.m_Bindings.Emplace(strRadius4, input::EKey::Numpad_4);
	layer.m_Bindings.Emplace(strRadius5, input::EKey::Numpad_5);
	layer.m_Bindings.Emplace(strRadius6, input::EKey::Numpad_6);
	layer.m_Bindings.Emplace(strSelect,  input::EKey::Mouse_Left, false);
	layer.m_Bindings.Emplace(strVoxel0,  input::EKey::Num_0);
	layer.m_Bindings.Emplace(strVoxel1,  input::EKey::Num_1);
	layer.m_Bindings.Emplace(strVoxel2,  input::EKey::Num_2);
	layer.m_Bindings.Emplace(strVoxel3,  input::EKey::Num_3);
	layer.m_Bindings.Emplace(strVoxel4,  input::EKey::Num_4);
	layer.m_Bindings.Emplace(strVoxel5,  input::EKey::Num_5);
	layer.m_Bindings.Emplace(strVoxel6,  input::EKey::Num_6);

	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strInput, layer);
}

void voxel::ModifySystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strInput);
}

void voxel::ModifySystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& transform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Segment3f segment = Segment3f(
			transform.m_Translate,
			transform.m_Translate +
			Vector3f::AxisZ * Quaternion::FromRotator(transform.m_Rotate) *
			100000.f);
		const Vector3f worldPos = Raycast(world, segment);
		const Vector3i gridPos = math::ToGridPos(worldPos, voxel::s_BlockSize1D);
		const Vector3f alignPos = math::ToWorldPos(gridPos, voxel::s_BlockSize1D);

		auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
		linesComponent.AddAABB(alignPos, voxel::s_BlockSize1D * 0.5f, Vector4f(1.f));

		{
			const auto& input = world.ReadResource<eng::InputManager>();
			auto& settingsComponent = world.WriteSingleton<voxel::ModifySettingsComponent>();
			if (input.IsPressed(strRadius0))
				settingsComponent.m_Radius = 0;
			if (input.IsPressed(strRadius1))
				settingsComponent.m_Radius = 1;
			if (input.IsPressed(strRadius2))
				settingsComponent.m_Radius = 2;
			if (input.IsPressed(strRadius3))
				settingsComponent.m_Radius = 3;
			if (input.IsPressed(strRadius4))
				settingsComponent.m_Radius = 4;
			if (input.IsPressed(strRadius5))
				settingsComponent.m_Radius = 5;
			if (input.IsPressed(strRadius6))
				settingsComponent.m_Radius = 6;

			if (input.IsPressed(strVoxel0))
				settingsComponent.m_Type = voxel::EType::None;
			if (input.IsPressed(strVoxel1))
				settingsComponent.m_Type = voxel::EType::Black;
			if (input.IsPressed(strVoxel2))
				settingsComponent.m_Type = voxel::EType::Green;
			if (input.IsPressed(strVoxel3))
				settingsComponent.m_Type = voxel::EType::Grey;
			if (input.IsPressed(strVoxel4))
				settingsComponent.m_Type = voxel::EType::Orange;
			if (input.IsPressed(strVoxel5))
				settingsComponent.m_Type = voxel::EType::Purple;
			if (input.IsPressed(strVoxel6))
				settingsComponent.m_Type = voxel::EType::Red;

			if (input.IsPressed(strSelect))
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
