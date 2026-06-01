#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace server::voxel
{
	void RegisterModule(ecs::EntityWorld& world);
}