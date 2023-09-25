#pragma once

#include <Core/Nullable.h>

#include <ECS/Component.h>
#include <ECS/System.h>

#include <Engine/FlipbookAsset.h>

namespace eng
{
	class AssetManager;
	struct FlipbookComponent;
	struct NameComponent;
}

namespace editor
{
	struct FlipbookWindowRequestComponent;

	struct FlipbookWindowComponent : public ecs::Component<FlipbookWindowComponent>
	{
		Nullable<eng::FlipbookAsset> m_Asset;

		str::String m_DockspaceLabel = {};
		str::String m_ExtractorLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_PopupOpenLabel = {};

		bool m_WasNewRequested = false;
		bool m_WasOpenRequested = false;
		bool m_WasSaveAsRequested = false;
		bool m_WasSaveRequested = false;
	};

	class FlipbookEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			editor::FlipbookWindowComponent,
			eng::AssetManager,
			eng::FlipbookComponent,
			eng::NameComponent,
			const editor::FlipbookWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
