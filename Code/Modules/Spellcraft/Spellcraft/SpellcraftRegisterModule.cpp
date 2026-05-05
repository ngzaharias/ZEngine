#include "SpellcraftPCH.h"
#include "Spellcraft/SpellcraftRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Spellcraft/SpellcraftBookComponent.h"
#include "Spellcraft/SpellcraftBookSystem.h"
#include "Spellcraft/SpellcraftCastComponent.h"
#include "Spellcraft/SpellcraftCastSystem.h"
#include "Spellcraft/SpellcraftDamageSystem.h"
#include "Spellcraft/SpellcraftExplosionComponent.h"
#include "Spellcraft/SpellcraftExplosionHitEvent.h"
#include "Spellcraft/SpellcraftExplosionSystem.h"
#include "Spellcraft/SpellcraftHealthComponent.h"
#include "Spellcraft/SpellcraftHealthSystem.h"
#include "Spellcraft/SpellcraftHealthTemplate.h"
#include "Spellcraft/SpellcraftProjectileComponent.h"
#include "Spellcraft/SpellcraftProjectileHitEvent.h"
#include "Spellcraft/SpellcraftProjectileSystem.h"

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