#pragma once

namespace ngraph
{
	struct Field;
	struct Link;
	struct Node;
}

namespace editor::spellcraft
{
	void PushFieldColour(const ngraph::Field& field);
	void PopFieldColour();

	void PushLinkColour(const ngraph::Link& link);
	void PopLinkColour();

	void PushNodeColour(const ngraph::Node& node);
	void PopNodeColour();
};