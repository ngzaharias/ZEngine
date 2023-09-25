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

	struct FlipbookExtractorComponent : public ecs::Component<FlipbookExtractorComponent>
	{
		Vector2u m_Dimensions = Vector2u(4, 1);
		Vector2f m_Initial = Vector2f::Zero;
		Vector2f m_Stride = Vector2f::Zero;

		Vector2f m_Size = Vector2f(100.f);
		int32 m_Count = 4;

		bool m_IsPreviewing = true;
	};
	struct FlipbookAssetOpenComponent : public ecs::Component<FlipbookAssetOpenComponent> { };
	struct FlipbookAssetSaveComponent : public ecs::Component<FlipbookAssetSaveComponent> { };

	struct FlipbookWindowComponent : public ecs::Component<FlipbookWindowComponent>
	{
		eng::FlipbookAsset m_Asset;

		str::String m_DockspaceLabel = {};
		str::String m_ExtractorLabel = {};
		str::String m_InspectorLabel = {};
		str::String m_PreviewerLabel = {};
		str::String m_TextureLabel = {};
	};

	class FlipbookEditor final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			editor::FlipbookAssetOpenComponent,
			editor::FlipbookAssetSaveComponent,
			editor::FlipbookExtractorComponent,
			editor::FlipbookWindowComponent,
			eng::AssetManager,
			eng::FlipbookComponent,
			eng::NameComponent,
			const editor::FlipbookWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
