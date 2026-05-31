#pragma once

#include "Engine/InputManager.h"

namespace shared::tactics
{
	enum EInputPriority
	{
		PawnSelection = eng::EInputPriority::Gameplay,
		AbilityPreview,
	};
}