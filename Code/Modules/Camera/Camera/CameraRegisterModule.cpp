#include "CameraPCH.h"
#include "Camera/CameraRegisterModule.h"

#include "Camera/CameraActivationSystem.h"
#include "Camera/CameraBound2DComponent.h"
#include "Camera/CameraBound2DSystem.h"
#include "Camera/CameraBound2DTemplate.h"
#include "Camera/CameraMove2DComponent.h"
#include "Camera/CameraMove2DSystem.h"
#include "Camera/CameraMove2DTemplate.h"
#include "Camera/CameraMove3DComponent.h"
#include "Camera/CameraMove3DSystem.h"
#include "Camera/CameraMove3DTemplate.h"
#include "Camera/CameraPan3DComponent.h"
#include "Camera/CameraPan3DSystem.h"
#include "Camera/CameraPan3DTemplate.h"
#include "Camera/CameraSettingsComponent.h"
#include "Camera/CameraZoom2DComponent.h"
#include "Camera/CameraZoom2DSystem.h"
#include "Camera/CameraZoom2DTemplate.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"

void camera::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<camera::Bound2DComponent>();
	world.RegisterComponent<camera::Bound2DTemplate>();
	world.RegisterComponent<camera::Move2DComponent>();
	world.RegisterComponent<camera::Move2DTemplate>();
	world.RegisterComponent<camera::Move3DComponent>();
	world.RegisterComponent<camera::Move3DTemplate>();
	world.RegisterComponent<camera::Pan3DComponent>();
	world.RegisterComponent<camera::Pan3DTemplate>();
	world.RegisterComponent<camera::SettingsComponent>();
	world.RegisterComponent<camera::Zoom2DComponent>();
	world.RegisterComponent<camera::Zoom2DTemplate>();
	world.RegisterSystem<camera::ActivationSystem>();
	world.RegisterSystem<camera::Bound2DSystem>();
	world.RegisterSystem<camera::Move2DSystem>();
	world.RegisterSystem<camera::Move3DSystem>();
	world.RegisterSystem<camera::Pan3DSystem>();
	world.RegisterSystem<camera::Zoom2DSystem>();

	// template
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<camera::Bound2DTemplate>();
		manager.RegisterComponent<camera::Move2DTemplate>();
		manager.RegisterComponent<camera::Move3DTemplate>();
		manager.RegisterComponent<camera::Pan3DTemplate>();
		manager.RegisterComponent<camera::Zoom2DTemplate>();
	}
}