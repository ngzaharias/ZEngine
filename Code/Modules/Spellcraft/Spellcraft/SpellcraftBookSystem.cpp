#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftBookSystem.h"

#include "Core/Guid.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Spellcraft/SpellcraftBookComponent.h"

namespace
{
	const str::Guid strDamage = str::Guid::Generate();
	const str::Guid strExplosion = str::Guid::Generate();
	const str::Guid strProjectile = GUID("e1967994f1424aa4afc4f1ee1743a16a");
}

void spellcraft::BookSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	auto& bookComponent = world.WriteComponent<spellcraft::BookComponent>();
	{
		spellcraft::Glyph& glyph = bookComponent.m_Glyph[strDamage];
		glyph = effect::Damage{ 
			.m_Amount = 1 };
	}
	{
		spellcraft::Glyph& glyph = bookComponent.m_Glyph[strExplosion];
		glyph = emitter::Explosion{ 
			.m_OnHit = strProjectile,
			.m_Radius = 1.f };
	}
	{
		spellcraft::Glyph& glyph = bookComponent.m_Glyph[strProjectile];
		glyph = emitter::Projectile{ 
			.m_OnHit = strExplosion, 
			.m_Distance = 500.f,
			.m_Speed = 200.f };
	}
}

void spellcraft::BookSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();
}
