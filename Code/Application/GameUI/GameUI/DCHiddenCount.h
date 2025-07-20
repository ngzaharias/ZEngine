#pragma once

#include "Engine/UIDataContext.h"

#include <NsApp/DelegateCommand.h>
#include <NsGui/BaseCommand.h>
#include <NsGui/INotifyPropertyChanged.h>

namespace gui
{
	class DCHiddenCount final : public eng::UIDataContext
	{
	public:
		DCHiddenCount();

		int32 GetObjectCount() const;
		void SetObjectCount(int32 value);

		int32 GetObjectTotal() const;
		void SetObjectTotal(int32 value);

	private:
		int32 m_ObjectCount = 0;
		int32 m_ObjectTotal = 0;

		NS_DECLARE_REFLECTION(gui::DCHiddenCount, eng::UIDataContext)
	};
}