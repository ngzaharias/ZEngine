#include "EntityEditorPCH.h"
#include "EntityEditor/EntityEditorCommands.h"

#include "Command/EntityCreate.h"
#include "Command/EntityDestroy.h"

editor::entity::Commands::Commands(ecs::EntityWorld& world)
	: History(world)
{ 
}

void editor::entity::Commands::Update()
{
	if (m_ExecStack.IsEmpty())
		return;

	for (Command* command : m_ExecStack)
		command->Exec(m_World);

	m_UndoStack.Append(m_ExecStack);
	m_ExecStack.RemoveAll();
	m_RedoStack.RemoveAll();
}

void editor::entity::Commands::CreateEntity(const str::StringView& name)
{
	m_ExecStack.Append(new EntityCreate(str::String(name)));
}

void editor::entity::Commands::DestroyEntity(const str::Guid& guid)
{
	m_ExecStack.Append(new EntityDestroy(guid));
}