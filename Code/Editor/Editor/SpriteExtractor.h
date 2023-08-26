#pragma once

#include <Core/Nullable.h>
#include <Core/Vector.h>

#include <Engine/System.h>
#include <Engine/Texture2DAsset.h>

namespace editor
{
	struct NamingSettings
	{
		int32 m_StartIndex = 0;
		str::String m_Template = "SP_Sprite_";
	};

	struct GridSettings
	{
		Vector2u m_Count = Vector2u::Zero;
		Vector2u m_Margin = Vector2u::Zero;
		Vector2u m_Size = Vector2u::Zero;
		Vector2u m_Spacing = Vector2u::Zero;
	};

	class SpriteExtractor final : public ecs::System
	{
	public:
		SpriteExtractor(asset::Manager& assetManager);

		void Update(const GameTime& gameTime) override;

		void OpenDialog(const str::Guid& guid);
		void CloseDialog();

	private:
		void Render();

		bool ExtractDialog();

	private:
		asset::Manager& m_AssetManager;

		Nullable<str::Guid> m_Selected = { };

		NamingSettings m_NamingSettings;
		GridSettings m_GridSettings;
	};
}