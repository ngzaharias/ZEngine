#pragma once

#include "Engine/UIDataContext.h"
#include "Math/Vector.h"

namespace gui
{
	class VMResolution final : public Noesis::BaseComponent
	{
	public:
		VMResolution(const Vector2u& resolution, bool isNative);

		const char* GetName() const;

	public:
		Noesis::String m_Name = {};
		Vector2u m_Resolution = {};

		NS_DECLARE_REFLECTION(gui::VMResolution, Noesis::BaseComponent)
	};
}