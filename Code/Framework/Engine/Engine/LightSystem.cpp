#include "EnginePCH.h"
#include "Engine/LightSystem.h"

#include "Core/Algorithms.h"
#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LightAmbientComponent.h"
#include "Engine/LightAmbientTemplate.h"
#include "Engine/LightDirectionalComponent.h"
#include "Engine/LightDirectionalTemplate.h"
#include "Engine/LightPointComponent.h"
#include "Engine/LightPointTemplate.h"

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;

	void ProcessAmbient(eng::LightSystem::World& world)
	{
		using AddedQuery = ecs::query
			::Added<const eng::light::AmbientTemplate>
			::Include<const eng::light::AmbientTemplate>;
		for (auto&& view : world.Query<AddedQuery>())
		{
			const auto& lightTemplate = view.ReadRequired<eng::light::AmbientTemplate>();
			auto& lightComponent = world.AddComponent<eng::light::AmbientComponent>(view);
			lightComponent.m_Colour = lightTemplate.m_Colour;
		}

		using UpdatedQuery = ecs::query
			::Updated<const eng::light::AmbientTemplate>
			::Include<eng::light::AmbientComponent, const eng::light::AmbientTemplate>;
		for (auto&& view : world.Query<UpdatedQuery>())
		{
			const auto& lightTemplate = view.ReadRequired<eng::light::AmbientTemplate>();
			auto& lightComponent = view.WriteRequired<eng::light::AmbientComponent>();
			lightComponent.m_Colour = lightTemplate.m_Colour;
		}

		for (auto&& view : world.Query<ecs::query::Removed<const eng::light::AmbientTemplate>>())
		{
			world.RemoveComponent<eng::light::AmbientComponent>(view);
		}
	}

	void ProcessDirectional(eng::LightSystem::World& world)
	{
		using AddedQuery = ecs::query
			::Added < const eng::light::DirectionalTemplate >
			::Include<const eng::light::DirectionalTemplate>;
		for (auto&& view : world.Query<AddedQuery>())
		{
			const auto& lightTemplate = view.ReadRequired<eng::light::DirectionalTemplate>();
			auto& lightComponent = world.AddComponent<eng::light::DirectionalComponent>(view);
			lightComponent.m_Colour = lightTemplate.m_Colour;
		}

		using UpdatedQuery = ecs::query
			::Updated<const eng::light::DirectionalTemplate>
			::Include<eng::light::DirectionalComponent, const eng::light::DirectionalTemplate>;
		for (auto&& view : world.Query<UpdatedQuery>())
		{
			const auto& lightTemplate = view.ReadRequired<eng::light::DirectionalTemplate>();
			auto& lightComponent = view.WriteRequired<eng::light::DirectionalComponent>();
			lightComponent.m_Colour = lightTemplate.m_Colour;
		}

		for (auto&& view : world.Query<ecs::query::Removed<const eng::light::DirectionalTemplate>>())
		{
			world.RemoveComponent<eng::light::DirectionalComponent>(view);
		}
	}

	void ProcessPoint(eng::LightSystem::World& world)
	{
		using AddedQuery = ecs::query
			::Added<const eng::light::PointTemplate>
			::Include<const eng::light::PointTemplate>;
		for (auto&& view : world.Query<AddedQuery>())
		{
			const auto& lightTemplate = view.ReadRequired<eng::light::PointTemplate>();
			auto& lightComponent = world.AddComponent<eng::light::PointComponent>(view);
			lightComponent.m_Colour = lightTemplate.m_Colour;
			lightComponent.m_Range = lightTemplate.m_Range;
		}

		using UpdatedQuery = ecs::query
			::Updated<const eng::light::PointTemplate>
			::Include<eng::light::PointComponent, const eng::light::PointTemplate>;
		for (auto&& view : world.Query<UpdatedQuery>())
		{
			const auto& lightTemplate = view.ReadRequired<eng::light::PointTemplate>();
			auto& lightComponent = view.WriteRequired<eng::light::PointComponent>();
			lightComponent.m_Colour = lightTemplate.m_Colour;
			lightComponent.m_Range = lightTemplate.m_Range;
		}

		for (auto&& view : world.Query<ecs::query::Removed<const eng::light::PointTemplate>>())
		{
			world.RemoveComponent<eng::light::PointComponent>(view);
		}
	}
}

void eng::LightSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	ProcessAmbient(world);
	ProcessDirectional(world);
	ProcessPoint(world);
}
