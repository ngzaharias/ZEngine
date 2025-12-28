#include "CameraPCH.h"
#include "Camera/RegisterModule.h"

#include "Camera/CameraBound2DComponent.h"
#include "Camera/CameraBound2DSystem.h"
#include "Camera/CameraMove2DComponent.h"
#include "Camera/CameraMove2DSystem.h"
#include "Camera/CameraMove3DComponent.h"
#include "Camera/CameraMove3DSystem.h"
#include "Camera/CameraPan3DComponent.h"
#include "Camera/CameraPan3DSystem.h"
#include "Camera/CameraZoom2DComponent.h"
#include "Camera/CameraZoom2DSystem.h"
#include "Camera/CameraSettingsSingleton.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"

void camera::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<camera::Bound2DComponent>();
		entityWorld.RegisterComponent<camera::Move2DComponent>();
		entityWorld.RegisterComponent<camera::Move3DComponent>();
		entityWorld.RegisterComponent<camera::Pan3DComponent>();
		entityWorld.RegisterComponent<camera::Zoom2DComponent>();
	}

	// singletons
	{
		entityWorld.RegisterSingleton<camera::SettingsSingleton>();
	}

	// systems
	{
		entityWorld.RegisterSystem<camera::Bound2DSystem>();
		entityWorld.RegisterSystem<camera::Move2DSystem>();
		entityWorld.RegisterSystem<camera::Move3DSystem>();
		entityWorld.RegisterSystem<camera::Pan3DSystem>();
		entityWorld.RegisterSystem<camera::Zoom2DSystem>();
	}

	// prototypes
	{
		auto& manager = entityWorld.WriteResource<eng::PrototypeManager>();
		manager.Register<camera::Bound2DComponent>();
		manager.Register<camera::Move2DComponent>();
		manager.Register<camera::Move3DComponent>();
		manager.Register<camera::Pan3DComponent>();
		manager.Register<camera::Zoom2DComponent>();
	}
}