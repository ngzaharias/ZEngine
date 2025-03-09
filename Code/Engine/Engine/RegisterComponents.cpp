#include "EnginePCH.h"
#include "Engine/RegisterComponents.h"

#include "Core/MemBuffer.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "Engine/ApplicationComponents.h"
#include "Engine/CameraComponent.h"
#include "Engine/ComponentSerializer.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/InputComponent.h"
#include "Engine/LevelComponents.h"
#include "Engine/LightComponents.h"
#include "Engine/LinesComponent.h"
#include "Engine/MusicComponents.h"
#include "Engine/NetworkComponents.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PrototypeManager.h"
#include "Engine/ReplicationComponents.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/SettingsComponents.h"
#include "Engine/SoundComponents.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/UserComponents.h"
#include "Engine/VersionComponent.h"
#include "Voxel/VoxelComponents.h"

#include <yojimbo/yojimbo.h>

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
		static void Read(ecs::NameComponent& component, const MemBuffer& buffer)
		{
			buffer.Read(component.m_Name);
		}

		static void Write(const ecs::NameComponent& component, MemBuffer& buffer)
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
	entityWorld.RegisterComponent<eng::application::CloseRequestComponent>();
	entityWorld.RegisterComponent<eng::DynamicMeshComponent>();
	entityWorld.RegisterComponent<eng::FlipbookAssetComponent>();
	entityWorld.RegisterComponent<eng::InputComponent>();
	entityWorld.RegisterComponent<eng::LightAmbientComponent>();
	entityWorld.RegisterComponent<eng::LightDirectionalComponent>();
	entityWorld.RegisterComponent<eng::LightPointComponent>();
	entityWorld.RegisterComponent<eng::network::RequestComponent>();
	entityWorld.RegisterComponent<eng::network::RequestFinishedComponent>();
	entityWorld.RegisterComponent<eng::RigidDynamicComponent>();
	entityWorld.RegisterComponent<eng::RigidStaticComponent>();
	entityWorld.RegisterComponent<eng::sound::ObjectComponent>();
	entityWorld.RegisterComponent<eng::sound::RandomComponent>();
	entityWorld.RegisterComponent<eng::sound::RandomRequestComponent>();
	entityWorld.RegisterComponent<eng::sound::SequenceComponent>();
	entityWorld.RegisterComponent<eng::sound::SequenceRequestComponent>();
	entityWorld.RegisterComponent<eng::sound::SingleRequestComponent>();
	entityWorld.RegisterComponent<eng::SpriteAssetComponent>();
	entityWorld.RegisterComponent<eng::StaticMeshAssetComponent>();
	entityWorld.RegisterComponent<eng::TextAssetComponent>();
	entityWorld.RegisterComponent<voxel::ChunkChangedEventComponent>();
	entityWorld.RegisterComponent<voxel::ChunkComponent>();
	entityWorld.RegisterComponent<voxel::ChunkLoadedEventComponent>();
	entityWorld.RegisterComponent<voxel::ModifyComponent>();

	entityWorld.RegisterSingleton<eng::FrameBufferComponent>();
	entityWorld.RegisterSingleton<eng::LinesComponent>();
	entityWorld.RegisterSingleton<eng::MusicComponent>();
	entityWorld.RegisterSingleton<eng::network::StateComponent>();
	entityWorld.RegisterSingleton<eng::settings::DebugComponent>();
	entityWorld.RegisterSingleton<eng::settings::GraphicsComponent>();
	entityWorld.RegisterSingleton<eng::settings::LaunchComponent>();
	entityWorld.RegisterSingleton<eng::settings::LocalComponent>();
	entityWorld.RegisterSingleton<eng::sound::RandomBufferComponent>();
	entityWorld.RegisterSingleton<eng::sound::SequenceBufferComponent>();
	entityWorld.RegisterSingleton<voxel::ModifySettingsComponent>();
}

void eng::RegisterServerComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSingleton<net::UserMapComponent>();
}

void eng::RegisterSharedComponents(ecs::EntityWorld& entityWorld, net::ComponentSerializer& serializer)
{
	// components
	entityWorld.RegisterComponent<eng::camera::Bound2DComponent>();
	entityWorld.RegisterComponent<eng::camera::Move2DComponent>();
	entityWorld.RegisterComponent<eng::camera::Move3DComponent>();
	entityWorld.RegisterComponent<eng::camera::Pan3DComponent>();
	entityWorld.RegisterComponent<eng::camera::Zoom2DComponent>();
	entityWorld.RegisterComponent<eng::camera::ProjectionComponent>();
	entityWorld.RegisterComponent<eng::FlipbookComponent>();
	entityWorld.RegisterComponent<eng::level::EntityComponent>();
	entityWorld.RegisterComponent<eng::level::LoadedComponent>();
	entityWorld.RegisterComponent<eng::level::LoadRequestComponent>();
	entityWorld.RegisterComponent<eng::level::UnloadRequestComponent>();
	entityWorld.RegisterComponent<eng::PhysicsComponent>();
	entityWorld.RegisterComponent<eng::PrototypeComponent>();
	entityWorld.RegisterComponent<eng::SpriteComponent>();
	entityWorld.RegisterComponent<eng::StaticMeshComponent>();
	entityWorld.RegisterComponent<eng::TextComponent>();
	entityWorld.RegisterComponent<eng::TransformComponent>();
	entityWorld.RegisterComponent<net::ReplicationComponent>();
	entityWorld.RegisterComponent<net::UserComponent>();

	// singletons
	entityWorld.RegisterSingleton<eng::level::DirectoryComponent>();
	entityWorld.RegisterSingleton<eng::PhysicsSceneComponent>();
	entityWorld.RegisterSingleton<eng::VersionComponent>();

	// serialization
	serializer.RegisterComponent<eng::camera::ProjectionComponent, Default<eng::camera::ProjectionComponent>>();
	serializer.RegisterComponent<ecs::NameComponent, NameSerializer>();
	serializer.RegisterComponent<eng::SpriteComponent, SpriteSerializer>();
	serializer.RegisterComponent<eng::TransformComponent, TransformSerializer>();
	serializer.RegisterComponent<net::UserComponent, UserSerializer>();
}
