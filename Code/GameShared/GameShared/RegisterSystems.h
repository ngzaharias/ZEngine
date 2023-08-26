#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace clt {}
namespace svr {}
namespace shd
{
	struct Dependencies
	{
	};

	void RegisterSystems(ecs::EntityWorld& entityWorld, const shd::Dependencies& dependencies);
}