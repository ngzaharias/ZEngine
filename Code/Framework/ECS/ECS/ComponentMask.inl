#pragma once

template<typename TComponent>
inline ecs::ComponentId ecs::ToComponentId()
{
	using NonConst = std::remove_const<TComponent>::type;
	return ToTypeId<NonConst, ecs::ComponentTag>();
}

template<typename ...TComponents>
inline ecs::ComponentMask ecs::ToComponentMask()
{
	ecs::ComponentMask componentMask;
	(componentMask.Raise(ToComponentId<TComponents>()), ...);
	return componentMask;
}

template<typename ...TComponents>
inline ecs::ComponentMask ecs::ToComponentMask(TypeList<TComponents...>)
{
	ecs::ComponentMask componentMask;
	(componentMask.Raise(ToComponentId<TComponents>()), ...);
	return componentMask;
}