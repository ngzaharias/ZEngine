#pragma once

#include "Core/Name.h"
#include "Core/MemBuffer.h"
#include "Core/String.h"
#include "Core/TypeInfo.h"
#include "ECS/QueryId.h"

class MemBuffer;

namespace ecs
{
	class EntityBuffer;
	class EntityStorage;
	struct Entity;
}

namespace ecs
{
	struct TypeComponent
	{
		str::Name m_Name = {};
		uint16 m_Bytes = 0;

		TypeId m_GlobalId = -1;
		TypeId m_LocalId = -1;

		ecs::QueryId m_AddedId = -1;
		ecs::QueryId m_RemovedId = -1;
		ecs::QueryId m_IncludeId = -1;

		bool m_IsReplicated = false;
		bool m_IsTemplate = false;

		using ConstructComponent = void(void*);
		ConstructComponent* m_ConstructComponent = nullptr;
		using CopystructComponent = void(void*, void*);
		CopystructComponent* m_CopystructComponent = nullptr;
		using DestructComponent = void(void*);
		DestructComponent* m_DestructComponent = nullptr;

		using HasComponent = bool(ecs::EntityStorage&, const ecs::Entity&);
		HasComponent* m_HasComponent = nullptr;
		using AddComponent = void(ecs::EntityBuffer&, const ecs::Entity&);
		AddComponent* m_AddComponent = nullptr;
		using RemoveComponent = void(ecs::EntityBuffer&, const ecs::Entity&);
		RemoveComponent* m_RemoveComponent = nullptr;

		using AddComponentData = void(ecs::EntityBuffer&, const ecs::Entity&, const MemBuffer&);
		AddComponentData* m_AddComponentData = nullptr;
		using ReadComponentData = void(ecs::EntityStorage&, const ecs::Entity&, MemBuffer&);
		ReadComponentData* m_ReadComponentData = nullptr;
		using WriteComponentData = void(ecs::EntityBuffer&, ecs::EntityStorage&, const ecs::Entity&, const MemBuffer&);
		WriteComponentData* m_WriteComponentData = nullptr;
	};

	template<typename TComponent>
	static void ConstructComponent(void* data);
	template<typename TComponent>
	static void CopystructComponent(void* source, void* target);
	template<typename TComponent>
	static void DestructComponent(void* data);

	template<typename TComponent>
	static bool HasComponent(ecs::EntityStorage& storage, const ecs::Entity& entity);
	template<typename TComponent>
	static void AddComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity);
	template<typename TComponent>
	static void AddComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity, const MemBuffer& data);
	template<typename TComponent>
	static void RemoveComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity);
	template<typename TComponent>
	static void ReadComponent(ecs::EntityStorage& storage, const ecs::Entity& entity, MemBuffer& data);
	template<typename TComponent>
	static void WriteComponent(ecs::EntityBuffer& buffer, ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data);
}

#include "ECS/TypeComponent.inl"