#pragma once

#include "Engine/UIDataContext.h"

#include <NsCore/DynamicCast.h>

template<typename TDataContext>
void eng::UIManager::RegisterDataContext(const str::Name& name)
{
	Noesis::Ptr<eng::UIDataContext> widget = Noesis::MakePtr<TDataContext>();
	widget->m_EntityWorld = &m_EntityWorld;
	m_DataContexts[name] = widget;
}

template<class TDataContext>
auto eng::UIManager::ReadDataContext(const str::Name& name)->const TDataContext&
{
	return *Noesis::DynamicCast<TDataContext*>(m_DataContexts[name].GetPtr());
}

template<class TDataContext>
auto eng::UIManager::WriteDataContext(const str::Name& name)->TDataContext&
{
	return *Noesis::DynamicCast<TDataContext*>(m_DataContexts[name].GetPtr());
}