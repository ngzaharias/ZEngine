#pragma once

#include "Engine/Asset.h"
#include "Engine/AssetLoader.h"

class Visitor;

namespace eng
{
	class AssetManager;
	class PhysicsManager;
}

namespace physx
{
	class PxMaterial;
}

namespace eng
{
	struct PhysicsMaterialAsset : public eng::Asset
	{
		physx::PxMaterial* m_Material = nullptr;
		float m_DynamicFriction = 0.f;
		float m_Restituation = 0.f;
		float m_StaticFriction = 0.f;
	};

	class PhysicsMaterialAssetLoader final : public eng::AssetLoader
	{
	public:
		PhysicsMaterialAssetLoader(eng::PhysicsManager& physicsManager);

		bool Load(eng::PhysicsMaterialAsset& asset, Visitor& visitor) const;
		bool Unload(eng::PhysicsMaterialAsset& asset) const;

	protected:
		eng::PhysicsManager& m_PhysicsManager;
	};
}