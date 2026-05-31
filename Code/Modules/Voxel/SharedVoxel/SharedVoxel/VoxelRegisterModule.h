#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace shared::voxel
{
	void RegisterModule(ecs::EntityWorld& world);
}