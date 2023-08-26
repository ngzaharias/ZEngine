#pragma once

#include <Engine/Asset.h>
#include <Engine/AssetLoader.h>

namespace eng
{
	class AssetManager;
	class PhysicsManager;
	class Visitor;
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

		bool Load(PhysicsMaterialAsset* asset, eng::Visitor& visitor) const;

	protected:
		eng::PhysicsManager& m_PhysicsManager;
	};
}