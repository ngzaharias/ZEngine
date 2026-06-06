#pragma once

#include "Engine/InputManager.h"

namespace client::tactics
{
	enum EInputPriority
	{
		PawnSelection = eng::EInputPriority::Gameplay,
		AbilityPreview,
	};
}