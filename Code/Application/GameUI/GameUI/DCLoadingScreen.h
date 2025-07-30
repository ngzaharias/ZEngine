#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>

namespace gui
{
	class DCLoadingScreen final : public eng::UIDataContext
	{
	public:
		DCLoadingScreen();
		~DCLoadingScreen() override;

		int32 GetProgress() const;
		void SetProgress(int32 value);

	private:
		int32 m_Progress = 0;

		NS_DECLARE_REFLECTION(gui::DCLoadingScreen, eng::UIDataContext)
	};
}