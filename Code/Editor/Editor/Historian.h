#pragma once

#include <Core/Guid.h>
#include <Core/RingQueue.h>

#include <Engine/TomlTypes.h>
#include <Engine/System.h>

namespace editor
{
	struct Transaction;

	struct Record
	{
		str::Guid m_Resource = { };
		toml::Value m_Document = { };
	};

	class Historian final : public ecs::System
	{
		using Records = RingQueue<Record, 50>;

	public:
		Historian() { }
		~Historian() { }

		void Initialise() override { }
		void Destroy() override { }

		void Update(const GameTime& gameTime) override;

		void PushChange(const Record& record);
		void RedoRecord();
		void UndoRecord();

		bool IsVisible() { return m_IsVisible; }
		void SetVisible(const bool value) { m_IsVisible = value; }

	private:
		void Render();

	private:
		Records m_UndoRecords = { };
		Records m_RedoRecords = { };

		bool m_IsVisible = false;
	};
}

#include "Editor/Historian.inl"