#pragma once

#include <Core/Nullable.h>

#include <Engine/SpriteAsset.h>
#include <Engine/System.h>

namespace editor
{
	class SpriteEditor final : public ecs::System
	{
	public:
		SpriteEditor(asset::Manager& assetManager);
		~SpriteEditor();

		void Initialise() override;
		void Destroy() override;

		void Update(const GameTime& gameTime) override;

		void OpenEditor(const str::Guid& guid);
		void CloseEditor();

	private:
		void Render();

	private:
		asset::Manager& m_AssetManager;

		Nullable<str::Guid> m_Selected = { };
	};
}
