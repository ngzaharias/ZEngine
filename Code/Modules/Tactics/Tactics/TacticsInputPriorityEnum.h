#pragma once

#include "Engine/InputManager.h"

namespace tactics
{
	enum EInputPriority
	{
		PawnSelection = eng::EInputPriority::Gameplay,
		AbilityPreview,
	};
}