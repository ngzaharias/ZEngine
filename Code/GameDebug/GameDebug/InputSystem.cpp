#include "GameDebugPCH.h"
#include "GameDebug/InputSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Input/Key.h"

#include "imgui/imgui.h"

namespace
{
	const str::Name strImGui = str::Name::Create("ImGui");
}

void dbg::InputSystem::Initialise(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.AppendLayer(strImGui, input::Layer{ eng::EInputPriority::Debug });
}

void dbg::InputSystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strImGui);
}

void dbg::InputSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& input = world.WriteResource<eng::InputManager>();
	input::Layer& layer = input.ModifyLayer(strImGui);
	const bool hasKeyboard = layer.m_Consume.Has(input::EConsume::Keyboard);
	const bool hasMouse = layer.m_Consume.Has(input::EConsume::Mouse);
	const bool wantsKeyboard = ImGui::GetIO().WantCaptureKeyboard;
	const bool wantsMouse = ImGui::GetIO().WantCaptureMouse;
	if (hasKeyboard != wantsKeyboard || hasMouse != wantsMouse)
	{
		layer.m_Consume.Set(input::EConsume::Keyboard, wantsKeyboard);
		layer.m_Consume.Set(input::EConsume::Mouse, wantsMouse);
	}
}
