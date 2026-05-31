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

void shared::spellcraft::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::spellcraft::BookComponent>();
	world.RegisterComponent<shared::spellcraft::CastComponent>();
	world.RegisterComponent<shared::spellcraft::ExplosionComponent>();
	world.RegisterComponent<shared::spellcraft::HealthComponent>();
	world.RegisterComponent<shared::spellcraft::HealthTemplate>();
	world.RegisterComponent<shared::spellcraft::ProjectileComponent>();
	world.RegisterEvent<shared::spellcraft::ExplosionHitEvent>();
	world.RegisterEvent<shared::spellcraft::ProjectileHitEvent>();
	world.RegisterSystem<shared::spellcraft::BookSystem>();
	world.RegisterSystem<shared::spellcraft::CastSystem>();
	world.RegisterSystem<shared::spellcraft::DamageSystem>();
	world.RegisterSystem<shared::spellcraft::ExplosionSystem>();
	world.RegisterSystem<shared::spellcraft::HealthSystem>();
	world.RegisterSystem<shared::spellcraft::ProjectileSystem>();
}