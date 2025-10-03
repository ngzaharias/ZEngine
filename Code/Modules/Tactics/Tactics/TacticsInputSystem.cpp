#include "TacticsPCH.h"
#include "Tactics/TacticsInputSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/InputManager.h"
#include "Tilemap/TilemapAgentComponent.h"
#include "Tactics/TacticsInput.h"

void tactics::InputSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Added<tilemap::AgentComponent>>())
	{
		input::Layer layer;
		layer.m_Priority = eng::EInputPriority::Gameplay;
		layer.m_Bindings.Emplace(strSelect, input::EKey::Mouse_Left, true);

		auto& input = world.WriteResource<eng::InputManager>();
		input.AppendLayer(strInput, layer);
	}

	if (world.HasAny<ecs::query::Removed<tilemap::AgentComponent>>())
	{
		auto& input = world.WriteResource<eng::InputManager>();
		input.RemoveLayer(strInput);
	}
}
