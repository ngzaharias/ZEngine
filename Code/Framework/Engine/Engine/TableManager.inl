#pragma once

#include "Engine/Visitor.h"

template<typename TKey, typename TValue>
void eng::TableManager<TKey, TValue>::Read(eng::Visitor& visitor)
{
	visitor.Read(m_ObjectMap);
}