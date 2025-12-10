#include "GameDebugPCH.h"
#include "GameDebug/InputSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/ImguiManager.h"
#include "Engine/InputManager.h"

namespace
{
	const str::Name strImGui = str::Name::Create("ImGui");
}

void debug::InputSystem::Initialise(World& world)
{
	auto& imgui = world.WriteResource<eng::ImguiManager>();
	auto& input = world.WriteResource<eng::InputManager>();

	input::Layer layer;
	layer.m_Priority = eng::EInputPriority::Debug;
	layer.m_Callback.Connect(imgui, &eng::ImguiManager::ProcessInput);
	input.AppendLayer(strImGui, layer);
}

void debug::InputSystem::Shutdown(World& world)
{
	auto& input = world.WriteResource<eng::InputManager>();
	input.RemoveLayer(strImGui);
}