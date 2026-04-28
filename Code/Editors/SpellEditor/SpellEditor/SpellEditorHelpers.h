#pragma once

namespace ngraph
{
	struct Field;
	struct Node;
}

namespace editor::spell
{
	void PushFieldColour(const ngraph::Field& field);
	void PopFieldColour();

	void PushNodeColour(const ngraph::Node& node);
	void PopNodeColour();
};