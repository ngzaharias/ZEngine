#pragma once

#include "Core/Colour.h"
#include "ECS/Component.h"

namespace hidden::settings
{
	/// \brief Debug settings that is only used in non-gold builds.
	struct DebugComponent : public ecs::SingletonComponent<DebugComponent>
	{
		bool m_IsInputEnabled = false;
		bool m_IsObjectEnabled = false;
	};

	/// \brief 
	struct LocalComponent : public ecs::SingletonComponent<LocalComponent>
	{
		Colour m_Background = Colour::White;
		Colour m_Highlight = Colour(0.54f, 0.54f, 0.91f);
		Colour m_Outline = Colour::Black;
	};
}