#pragma once

#include <Core/Array.h>

#include <ECS/Entity.h>
#include <ECS/System.h>

namespace container
{
	struct MemberChangesComponent;
	struct OwnerComponent;
	struct StorageComponent;
	struct StorageCreateRequestComponent;
	struct StorageCreateResultComponent;
	struct StorageDestroyRequestComponent;
	struct StorageDestroyResultComponent;

	struct StorageData
	{
		ecs::Entity m_Storage = {};
		ecs::Entity m_Owner = {};
		int32 m_Type = -1;
	};

	struct FrameData
	{
		Array<StorageData> m_StorageCreated;
		Array<StorageData> m_StorageDestroyed;
	};

	/// \brief Handles creating/destroying of storage entity and its components.
	class StorageSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			container::StorageComponent,
			container::StorageCreateResultComponent,
			container::StorageDestroyResultComponent,
			const container::MemberChangesComponent,
			const container::OwnerComponent,
			const container::StorageCreateRequestComponent,
			const container::StorageDestroyRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void ProcessMemberChanges(World& world, FrameData& frameData);
		void ProcessOwnerRequests(World& world, FrameData& frameData);
		void ProcessStorageRequests(World& world, FrameData& frameData);
	};
}