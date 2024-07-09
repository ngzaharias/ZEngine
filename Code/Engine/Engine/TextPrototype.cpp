#include "EnginePCH.h"
#include "Engine/TextPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/TextComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strText = "m_Text";
	const str::StringView strFont = "m_Font";
}

void eng::TextLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::TextPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::TextComponent>(entity);
	component.m_Text = prototype.m_Text;
	component.m_Font = prototype.m_Font;
}

void eng::TextLoader::Load(eng::TextPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Visit(strText, prototype.m_Text, {});
	visitor.Visit(strFont, prototype.m_Font, {});
}