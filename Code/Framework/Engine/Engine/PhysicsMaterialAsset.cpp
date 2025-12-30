#include "EnginePCH.h"
#include "Engine/PhysicsMaterialAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/Visitor.h"

#include <PhysX/PxMaterial.h>
#include <PhysX/PxPhysics.h>

namespace
{
	const str::Name strDynamicFriction = NAME("m_DynamicFriction");
	const str::Name strRestituation = NAME("m_Restituation");
	const str::Name strStaticFriction = NAME("m_StaticFriction");
}

eng::PhysicsMaterialAssetLoader::PhysicsMaterialAssetLoader(eng::PhysicsManager& physicsManager)
	: m_PhysicsManager(physicsManager) 
{ 
}

bool eng::PhysicsMaterialAssetLoader::Load(eng::PhysicsMaterialAsset& asset, eng::Visitor& visitor) const
{
	PROFILE_FUNCTION();

	visitor.Read(strDynamicFriction, asset.m_DynamicFriction, 0.f);
	visitor.Read(strRestituation, asset.m_Restituation, 0.f);
	visitor.Read(strStaticFriction, asset.m_StaticFriction, 0.f);

	physx::PxPhysics& physics = m_PhysicsManager.GetPhysics();
	asset.m_Material = physics.createMaterial(
		asset.m_StaticFriction, 
		asset.m_DynamicFriction, 
		asset.m_Restituation);
	return true;
}

bool eng::PhysicsMaterialAssetLoader::Unload(eng::PhysicsMaterialAsset& asset) const
{
	PROFILE_FUNCTION();
	asset.m_Material->release();
	return true;
}