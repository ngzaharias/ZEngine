#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/IsReplicated.h"

namespace eng
{
	struct UUIDComponent final : public ecs::Component
		, ecs::IsReplicated
	{
		UUIDComponent() = default;
		UUIDComponent(const str::Guid& value) : m_UUID(value) { }

		str::Guid m_UUID = {};
	};
}