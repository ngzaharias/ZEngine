#pragma once

#include <Core/StringView.h>

#include <Engine/System.h>

#include <Editor/Node.h>

namespace editor
{
	class NodeGraphEditor final : public ecs::System
	{
	public:
		void Initialise() override;
		void Update(const GameTime& gameTime) override;

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void CreateNode(const str::StringView& label);
		void DestroyNode(const node::Node& node);

	private:
		int32 m_HandleLast = 1;
		Array<int32> m_HandlesFree = { };

		Array<std::pair<int, int>> m_Links = { };
		Array<node::Node> m_Nodes = { };

		bool m_IsVisible = true;
	};
}