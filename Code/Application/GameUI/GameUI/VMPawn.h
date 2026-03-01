#pragma once

#include "Core/String.h"
#include "ECS/Entity.h"
#include "Engine/UIViewModel.h"
#include "GameUI/VMAbility.h"

namespace Noesis
{
	template<class T>
	class ObservableCollection;
}

namespace gui
{
	class VMPawn final : public eng::UIViewModel
	{
	public:
		VMPawn(const ecs::Entity& entity, const str::String& name);

		const ecs::Entity& GetEntity() const { return m_Entity; }
		const char* GetName() const { return m_Name.c_str(); }

		Noesis::ObservableCollection<gui::VMAbility>* GetAbilities() const;
		void SetAbilities(Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> value);

	private:
		ecs::Entity m_Entity = {};
		str::String m_Name = {};

		Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> m_Abilities;

	private:
		NS_DECLARE_REFLECTION(gui::VMPawn, Noesis::BaseComponent)
	};
}