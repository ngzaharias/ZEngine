#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "EntityEditor/EntityEditorCommands.h"

#include "imgui/imgui.h"

namespace editor::entity
{
	class Inspector
	{
	public:
		template<typename Value>
		struct IsInline : std::false_type {};

	public:
		Inspector() = default;
		~Inspector();

		bool Begin(const char* label, const str::Guid& entityUUID);
		void End();

		bool HasCommands() const;
		auto GetCommands() const -> const Array<Command*>;
		auto MoveCommands() -> Array<Command*>;

		template<typename Value>
		void Visit(const Value& value);
		template<typename Value>
		void Visit(const char* label, const Value& value);

	protected:
		template<typename Value>
		inline void VisitCustom(const Value& value);

	protected:
		template<typename TObject, typename TValue>
		void PushCommand(TValue TObject::* fieldPtr, const TValue& valueOld, const TValue& valueNew);

	protected:
		str::Guid m_EntityUUID = {};
		Array<Command*> m_Commands = {};
	};
}

#include "EntityEditorInspector.inl"