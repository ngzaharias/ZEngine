#pragma once

#include <Engine/Visitor.h>

template<typename TKey, typename TValue>
void eng::TableManager<TKey, TValue>::Visit(eng::Visitor& visitor)
{
	visitor.Visit(m_ObjectMap);
}