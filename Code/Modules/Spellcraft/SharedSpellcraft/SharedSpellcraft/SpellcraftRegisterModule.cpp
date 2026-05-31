#include "SpellcraftPCH.h"
#include "SharedSpellcraft/SpellcraftRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "SharedSpellcraft/SpellcraftBookComponent.h"
#include "SharedSpellcraft/SpellcraftBookSystem.h"
#include "SharedSpellcraft/SpellcraftCastComponent.h"
#include "SharedSpellcraft/SpellcraftCastSystem.h"
#include "SharedSpellcraft/SpellcraftDamageSystem.h"
#include "SharedSpellcraft/SpellcraftExplosionComponent.h"
#include "SharedSpellcraft/SpellcraftExplosionHitEvent.h"
#include "SharedSpellcraft/SpellcraftExplosionSystem.h"
#include "SharedSpellcraft/SpellcraftHealthComponent.h"
#include "SharedSpellcraft/SpellcraftHealthSystem.h"
#include "SharedSpellcraft/SpellcraftHealthTemplate.h"
#include "SharedSpellcraft/SpellcraftProjectileComponent.h"
#include "SharedSpellcraft/SpellcraftProjectileHitEvent.h"
#include "SharedSpellcraft/SpellcraftProjectileSystem.h"

void spellcraft::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<spellcraft::BookComponent>();
	world.RegisterComponent<spellcraft::CastComponent>();
	world.RegisterComponent<spellcraft::ExplosionComponent>();
	world.RegisterComponent<spellcraft::HealthComponent>();
	world.RegisterComponent<spellcraft::HealthTemplate>();
	world.RegisterComponent<spellcraft::ProjectileComponent>();
	world.RegisterEvent<spellcraft::ExplosionHitEvent>();
	world.RegisterEvent<spellcraft::ProjectileHitEvent>();
	world.RegisterSystem<spellcraft::BookSystem>();
	world.RegisterSystem<spellcraft::CastSystem>();
	world.RegisterSystem<spellcraft::DamageSystem>();
	world.RegisterSystem<spellcraft::ExplosionSystem>();
	world.RegisterSystem<spellcraft::HealthSystem>();
	world.RegisterSystem<spellcraft::ProjectileSystem>();
}