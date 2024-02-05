#include "EnginePCH.h"
#include "Engine/RegisterComponents.h"

#include <Core/MemBuffer.h>

#include <ECS/EntityWorld.h>

#include <yojimbo/yojimbo.h>

#include "Engine/CameraComponent.h"
#include "Engine/ComponentSerializer.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/InputComponent.h"
#include "Engine/LevelComponents.h"
#include "Engine/LightComponents.h"
#include "Engine/LinesComponent.h"
#include "Engine/NameComponent.h"
#include "Engine/NetworkComponents.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/ReplicationComponents.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/UserComponents.h"
#include "Engine/VoxelComponents.h"

namespace
{
	template<typename TComponent>
	struct Default final
	{
		static void Read(TComponent& component, const MemBuffer& buffer) { }
		static void Write(const TComponent& component, MemBuffer& buffer) { }
	};

	struct NameSerializer final
	{
		static void Read(eng::NameComponent& component, const MemBuffer& buffer)
		{
			buffer.Read(component.m_Name);
		}

		static void Write(const eng::NameComponent& component, MemBuffer& buffer)
		{
			buffer.Write(component.m_Name);
		}
	};

	struct SpriteSerializer final
	{
		static void Read(eng::SpriteComponent& component, const MemBuffer& buffer)
		{
			buffer.Read(component.m_Size.x);
			buffer.Read(component.m_Size.y);
			buffer.Read(component.m_Sprite.m_Data);
		}

		static void Write(const eng::SpriteComponent& component, MemBuffer& buffer)
		{
			buffer.Write(component.m_Size.x);
			buffer.Write(component.m_Size.y);
			buffer.Write(component.m_Sprite.m_Data);
		}
	};

	struct TransformSerializer final
	{
		static void Read(eng::TransformComponent& component, const MemBuffer& buffer)
		{
			buffer.Read(component.m_Translate.x);
			buffer.Read(component.m_Translate.y);
			buffer.Read(component.m_Translate.z);

			buffer.Read(component.m_Rotate.m_Pitch);
			buffer.Read(component.m_Rotate.m_Yaw);
			buffer.Read(component.m_Rotate.m_Roll);

			buffer.Read(component.m_Scale.x);
			buffer.Read(component.m_Scale.y);
			buffer.Read(component.m_Scale.z);
		}

		static void Write(const eng::TransformComponent& component, MemBuffer& buffer)
		{
			buffer.Write(component.m_Translate.x);
			buffer.Write(component.m_Translate.y);
			buffer.Write(component.m_Translate.z);

			buffer.Write(component.m_Rotate.m_Pitch);
			buffer.Write(component.m_Rotate.m_Yaw);
			buffer.Write(component.m_Rotate.m_Roll);

			buffer.Write(component.m_Scale.x);
			buffer.Write(component.m_Scale.y);
			buffer.Write(component.m_Scale.z);
		}
	};

	struct UserSerializer final
	{
		static void Read(net::UserComponent& component, const MemBuffer& buffer)
		{
			buffer.Read(component.m_UserId.m_Value);
		}

		static void Write(const net::UserComponent& component, MemBuffer& buffer)
		{
			buffer.Write(component.m_UserId.m_Value);
		}
	};
}

void eng::RegisterClientComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<eng::DynamicMeshComponent>();
	entityWorld.RegisterComponent<eng::FlipbookComponent>();
	entityWorld.RegisterComponent<eng::FrameBufferComponent>();
	entityWorld.RegisterComponent<eng::InputComponent>();
	entityWorld.RegisterComponent<eng::LightAmbientComponent>();
	entityWorld.RegisterComponent<eng::LightDirectionalComponent>();
	entityWorld.RegisterComponent<eng::LightPointComponent>();
	entityWorld.RegisterComponent<eng::LinesComponent>();
	entityWorld.RegisterComponent<eng::RigidDynamicComponent>();
	entityWorld.RegisterComponent<eng::RigidStaticComponent>();
	entityWorld.RegisterComponent<eng::StaticMeshComponent>();
	entityWorld.RegisterComponent<eng::TextComponent>();
	entityWorld.RegisterComponent<network::RequestComponent>();
	entityWorld.RegisterComponent<network::RequestFinishedComponent>();
	entityWorld.RegisterComponent<network::StateComponent>();
	entityWorld.RegisterComponent<voxel::ChunkChangedEventComponent>();
	entityWorld.RegisterComponent<voxel::ChunkComponent>();
	entityWorld.RegisterComponent<voxel::ChunkLoadedEventComponent>();
	entityWorld.RegisterComponent<voxel::ModifyComponent>();
	entityWorld.RegisterComponent<voxel::ModifySettingsComponent>();
}

void eng::RegisterServerComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<net::UserMapComponent>();
}

void eng::RegisterSharedComponents(ecs::EntityWorld& entityWorld, net::ComponentSerializer& serializer)
{
	entityWorld.RegisterComponent<eng::CameraComponent>();
	entityWorld.RegisterComponent<eng::LevelDirectoryComponent>();
	entityWorld.RegisterComponent<eng::LevelEntityComponent>();
	entityWorld.RegisterComponent<eng::LevelLoadedComponent>();
	entityWorld.RegisterComponent<eng::LevelLoadRequestComponent>();
	entityWorld.RegisterComponent<eng::LevelUnloadRequestComponent>();
	entityWorld.RegisterComponent<eng::NameComponent>();
	entityWorld.RegisterComponent<eng::PhysicsSceneComponent>();
	entityWorld.RegisterComponent<eng::SpriteComponent>();
	entityWorld.RegisterComponent<eng::TransformComponent>();
	entityWorld.RegisterComponent<net::ReplicationComponent>();
	entityWorld.RegisterComponent<net::UserComponent>();

	serializer.RegisterComponent<eng::CameraComponent, Default<eng::CameraComponent>>();
	serializer.RegisterComponent<eng::NameComponent,      NameSerializer>();
	serializer.RegisterComponent<eng::SpriteComponent,    SpriteSerializer>();
	serializer.RegisterComponent<eng::TransformComponent, TransformSerializer>();
	serializer.RegisterComponent<net::UserComponent,      UserSerializer>();
}
