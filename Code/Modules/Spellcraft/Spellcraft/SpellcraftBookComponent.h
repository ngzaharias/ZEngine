#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Variant.h"
#include "ECS/Component.h"

namespace spellcraft::emitter
{
	struct Explosion 
	{
		str::Guid m_OnHit = {};
		float m_Radius = 0.f;
		float m_Speed = 0.f;
	};
	struct Projectile 
	{
		str::Guid m_OnHit = {};
		float m_Distance = 0.f;
		float m_Speed = 0.f;
	};
}

namespace spellcraft::effect
{
	struct Damage 
	{
		int32 m_Amount = 0;
	};
	struct Heal 
	{
		int32 m_Amount = 0;
	};
}

namespace spellcraft
{
	using Glyph = Variant<
		effect::Damage,
		effect::Heal,
		emitter::Explosion,
		emitter::Projectile>;

	struct BookComponent final : public ecs::StaticComponent
	{
		Map<str::Guid, Glyph> m_Glyph = {};
	};
}