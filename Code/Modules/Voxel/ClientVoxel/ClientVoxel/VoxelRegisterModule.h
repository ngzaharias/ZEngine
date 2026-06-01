#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace client::voxel
{
	void RegisterModule(ecs::EntityWorld& world);
}