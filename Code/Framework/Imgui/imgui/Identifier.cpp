#include "imgui/Identifier.h"

int32 imgui::Identifier::Borrow()
{
	int32 i = 0;
	for (bool id : m_Ids)
	{
		if (!id)
		{
			m_Ids[i] = true;
			return i;
		}
		i++;
	}

	m_Ids.push_back(true);
	return static_cast<int32>(m_Ids.size() - 1);
}

void imgui::Identifier::Release(int32 id)
{
	m_Ids[id] = false;
}