#pragma once

#include "Core/Array.h"
#include "Core/Optional.h"
#include "Core/Path.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "ImportEditor/ImportMeshPopup.h"

namespace editor::importer
{
	using Popup = Variant<
		editor::importer::MeshPopup
	>;

	struct WindowComponent final : public ecs::SoloComponent
	{
		Array<str::Path> m_Files = {};
		Optional<Popup> m_Popup = {};
	};
}
