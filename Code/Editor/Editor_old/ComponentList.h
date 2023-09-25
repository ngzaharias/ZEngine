#pragma once

#include <Editor/ExampleComponent.h>

#include <Core/TypeList.h>

#include <Engine/CameraComponent.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/InputComponent.h>
#include <Engine/NameComponent.h>
#include <Engine/RigidDynamicComponent.h>
#include <Engine/RigidStaticComponent.h>
#include <Engine/SoundComponent.h>
#include <Engine/SpriteComponent.h>
#include <Engine/TransformComponent.h>

namespace editor
{
	using ComponentsList = core::TypeList
		<
		// #note: always name, then transform
		core::NameComponent
		, core::TransformComponent
		
		// #todo: order these somehow?
		, audio::SoundComponent
		, example::ExampleComponent
		, input::InputComponent
		, physics::RigidDynamicComponent
		, physics::RigidStaticComponent
		, render::FlipbookComponent
		, render::SpriteComponent
		>;

	constexpr ComponentsList s_ComponentList;
}