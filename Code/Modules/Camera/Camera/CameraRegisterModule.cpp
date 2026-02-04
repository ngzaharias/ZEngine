#include "CameraPCH.h"
#include "Camera/CameraRegisterModule.h"

#include "Camera/CameraActivationSystem.h"
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

void camera::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<camera::Bound2DComponent>();
		world.RegisterComponent<camera::Move2DComponent>();
		world.RegisterComponent<camera::Move3DComponent>();
		world.RegisterComponent<camera::Pan3DComponent>();
		world.RegisterComponent<camera::Zoom2DComponent>();
	}

	// singletons
	{
		world.RegisterSingleton<camera::SettingsSingleton>();
	}

	// systems
	{
		world.RegisterSystem<camera::ActivationSystem>();
		world.RegisterSystem<camera::Bound2DSystem>();
		world.RegisterSystem<camera::Move2DSystem>();
		world.RegisterSystem<camera::Move3DSystem>();
		world.RegisterSystem<camera::Pan3DSystem>();
		world.RegisterSystem<camera::Zoom2DSystem>();
	}

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.Register<camera::Bound2DComponent>();
		manager.Register<camera::Move2DComponent>();
		manager.Register<camera::Move3DComponent>();
		manager.Register<camera::Pan3DComponent>();
		manager.Register<camera::Zoom2DComponent>();
	}
}