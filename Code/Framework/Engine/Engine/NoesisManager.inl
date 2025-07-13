#pragma once

template<typename TDataContext>
void eng::UIManager::RegisterDataContext(const str::Name& name)
{
	m_DataContexts[name] = Noesis::MakePtr<TDataContext>();
}