#include "CorePCH.h"

//template<typename Type, typename Field>
//void ngraph::NodeRegistry::RegisterInput(NodeEntry& entry, const Type, const Field)
//{
//}
//
//template<typename Type, typename... Fields>
//void ngraph::NodeRegistry::RegisterInputs(NodeEntry& entry, const Type, const TypeList<Fields...>)
//{
//	(RegisterInput(entry, Type{}, Fields{}), ...);
//}
//
//template<>
//struct TypeMeta<TestNode>
//{
//	using Inputs = TypeList<
//		FieldMeta<&TestNode::m_Bool>,
//		FieldMeta<&TestNode::m_Float>
//	>;
//
//	using Outputs = TypeList<
//		FieldMeta<&TestNode::m_Integer>
//	>;
//};