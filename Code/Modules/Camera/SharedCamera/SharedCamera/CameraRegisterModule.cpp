#include "CameraPCH.h"
#include "SharedCamera/CameraRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedCamera/CameraActivationSystem.h"
#include "SharedCamera/CameraBound2DComponent.h"
#include "SharedCamera/CameraBound2DSystem.h"
#include "SharedCamera/CameraBound2DTemplate.h"
#include "SharedCamera/CameraMove2DComponent.h"
#include "SharedCamera/CameraMove2DSystem.h"
#include "SharedCamera/CameraMove2DTemplate.h"
#include "SharedCamera/CameraMove3DComponent.h"
#include "SharedCamera/CameraMove3DSystem.h"
#include "SharedCamera/CameraMove3DTemplate.h"
#include "SharedCamera/CameraPan3DComponent.h"
#include "SharedCamera/CameraPan3DSystem.h"
#include "SharedCamera/CameraPan3DTemplate.h"
#include "SharedCamera/CameraSettingsComponent.h"
#include "SharedCamera/CameraZoom2DComponent.h"
#include "SharedCamera/CameraZoom2DSystem.h"
#include "SharedCamera/CameraZoom2DTemplate.h"

void shared::camera::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::camera::Bound2DComponent>();
	world.RegisterComponent<shared::camera::Bound2DTemplate>();
	world.RegisterComponent<shared::camera::Move2DComponent>();
	world.RegisterComponent<shared::camera::Move2DTemplate>();
	world.RegisterComponent<shared::camera::Move3DComponent>();
	world.RegisterComponent<shared::camera::Move3DTemplate>();
	world.RegisterComponent<shared::camera::Pan3DComponent>();
	world.RegisterComponent<shared::camera::Pan3DTemplate>();
	world.RegisterComponent<shared::camera::SettingsComponent>();
	world.RegisterComponent<shared::camera::Zoom2DComponent>();
	world.RegisterComponent<shared::camera::Zoom2DTemplate>();
	world.RegisterSystem<shared::camera::ActivationSystem>();
	world.RegisterSystem<shared::camera::Bound2DSystem>();
	world.RegisterSystem<shared::camera::Move2DSystem>();
	world.RegisterSystem<shared::camera::Move3DSystem>();
	world.RegisterSystem<shared::camera::Pan3DSystem>();
	world.RegisterSystem<shared::camera::Zoom2DSystem>();

	// template
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::camera::Bound2DTemplate>();
		manager.RegisterComponent<shared::camera::Move2DTemplate>();
		manager.RegisterComponent<shared::camera::Move3DTemplate>();
		manager.RegisterComponent<shared::camera::Pan3DTemplate>();
		manager.RegisterComponent<shared::camera::Zoom2DTemplate>();
	}
}