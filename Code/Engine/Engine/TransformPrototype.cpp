#include "EnginePCH.h"
#include "Engine/TransformPrototype.h"

#include "ECS/EntityWorld.h"
#include "Engine/TransformComponent.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strTranslate = "m_Translate";
	const str::StringView strRotate = "m_Rotate";
	const str::StringView strScale = "m_Scale";
}

void eng::TransformLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::TransformPrototype& prototype) const
{
	auto& component = world.AddComponent<eng::TransformComponent>(entity);
	component.m_Translate = prototype.m_Translate;
	component.m_Rotate = prototype.m_Rotate;
	component.m_Scale = prototype.m_Scale;
}

void eng::TransformLoader::Load(eng::TransformPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strTranslate, prototype.m_Translate, Vector3f::Zero);
	visitor.Read(strRotate, prototype.m_Rotate, Rotator::Zero);
	visitor.Read(strScale, prototype.m_Scale, Vector3f::One);
}