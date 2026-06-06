#pragma once

namespace ecs
{
	class EntityWorld;
}

namespace client::tactics
{
	void RegisterModule(ecs::EntityWorld& world);
}