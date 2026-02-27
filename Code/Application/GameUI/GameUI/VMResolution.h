#pragma once

#include "Engine/UIViewModel.h"
#include "Math/Vector.h"

namespace gui
{
	class VMResolution final : public eng::UIViewModel
	{
	public:
		VMResolution(const Vector2u& resolution, bool isNative);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		Vector2u m_Resolution = {};

	private:
		NS_DECLARE_REFLECTION(gui::VMResolution, Noesis::BaseComponent)
	};
}