#pragma once

inline ecs::ComponentMask ecs::ComponentMask::operator|(const ComponentMask& rhs) const
{
	ecs::ComponentMask lhs = *this;
	lhs.m_Data |= rhs.m_Data;
	return lhs;
}

inline ecs::ComponentMask ecs::ComponentMask::operator&(const ComponentMask& rhs) const
{
	ecs::ComponentMask lhs = *this;
	lhs.m_Data &= rhs.m_Data;
	return lhs;
}

inline ecs::ComponentMask ecs::ComponentMask::operator^(const ComponentMask& rhs) const
{
	ecs::ComponentMask lhs = *this;
	lhs.m_Data ^= rhs.m_Data;
	return lhs;
}

inline ecs::ComponentMask ecs::ComponentMask::operator~() const
{
	ecs::ComponentMask lhs = *this;
	lhs.m_Data.flip();
	return lhs;
}

inline ecs::ComponentMask& ecs::ComponentMask::operator|=(const ComponentMask& rhs)
{
	m_Data |= rhs.m_Data;
	return *this;
}

inline ecs::ComponentMask& ecs::ComponentMask::operator&=(const ComponentMask& rhs)
{
	m_Data &= rhs.m_Data;
	return *this;
}

inline ecs::ComponentMask& ecs::ComponentMask::operator^=(const ComponentMask& rhs)
{
	m_Data ^= rhs.m_Data;
	return *this;
}

inline void ecs::ComponentMask::Reset()
{
	m_Data.reset();
}

inline void ecs::ComponentMask::Clear(const int32 index)
{
	m_Data.set(index, false);
}

inline void ecs::ComponentMask::Raise(const int32 index)
{
	m_Data.set(index, true);
}

inline bool ecs::ComponentMask::Has(const int32 index) const
{
	return m_Data.test(index);
}

inline bool ecs::ComponentMask::HasAll() const
{
	return m_Data.all();
}

inline bool ecs::ComponentMask::HasAny() const
{
	return m_Data.any();
}

inline bool ecs::ComponentMask::HasNone() const
{
	return m_Data.none();
}

inline bool ecs::ComponentMask::HasAll(const ComponentMask& rhs) const
{
	return (rhs.m_Data & m_Data) == rhs.m_Data;
}

inline bool ecs::ComponentMask::HasAny(const ComponentMask& rhs) const
{
	return (rhs.m_Data & m_Data).any();
}

inline bool ecs::ComponentMask::HasNone(const ComponentMask& rhs) const
{
	return !HasAny(rhs);
}

//////////////////////////////////////////////////////////////////////////

template<typename TComponent>
inline int32 ecs::ToComponentId()
{
	using NonConst = std::remove_const<TComponent>::type;
	return ToTypeIndex<NonConst, ecs::ComponentTag>();
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