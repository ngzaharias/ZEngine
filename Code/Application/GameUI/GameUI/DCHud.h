#pragma once

#include "ECS/WorldView.h"
#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>

namespace eng
{
	class AbilityTable;
}

namespace gui
{
	class VMAbility;
}

namespace Noesis
{
	template<class T>
	class ObservableCollection;
}

namespace gui
{
	class DCHud final : public eng::UIDataContext
	{
	public:
		using World = ecs::WorldView
			::Read<
			eng::AbilityTable>;

		DCHud();
		~DCHud() override;

		void Initialise(World& world);

		Noesis::ObservableCollection<gui::VMAbility>* GetAbilities() const;
		void SetAbilities(Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> value);

	private:
		Noesis::Ptr<Noesis::ObservableCollection<gui::VMAbility>> m_Abilities;

		NS_DECLARE_REFLECTION(gui::DCHud, eng::UIDataContext)
	};
}