#include "Editor_gen/TestProperty.h"

codegen::TestProperty::TestProperty() noexcept
	: kodgen::MacroPropertyCodeGen("Test", kodgen::EEntityType::Field)
{
}

bool codegen::TestProperty::preGenerateCodeForEntity(
	const kodgen::EntityInfo& entity,
	const kodgen::Property& property,
	kodgen::uint8 propertyIndex,
	kodgen::MacroCodeGenEnv& env) noexcept
{
	std::string errorMessage;

	//Can't have * and & at the same time
	if (std::find(property.arguments.cbegin(), property.arguments.cend(), "*") != property.arguments.cend() &&
		std::find(property.arguments.cbegin(), property.arguments.cend(), "&") != property.arguments.cend())
	{
		errorMessage = "Get property can't accept both '*' and '&' at the same time.";
	}
	else
	{
		//Check that Get property arguments are valid
		for (std::string const& arg : property.arguments)
		{
			if (arg != "*" && arg != "&" && arg != "const" && arg != "explicit")
			{
				errorMessage = "Get property only accepts '*', '&' and 'explicit' arguments.";
				break;
			}
		}
	}

	if (!errorMessage.empty())
	{
		//Log error message and abort generation
		if (env.getLogger() != nullptr)
		{
			env.getLogger()->log(errorMessage, kodgen::ILogger::ELogSeverity::Error);
		}

		return false;
	}

	//If arguments are valid, dispatch the generation call normally
	return true;
}

bool codegen::TestProperty::generateClassFooterCodeForEntity(
	const kodgen::EntityInfo& entity,
	const kodgen::Property& property,
	kodgen::uint8 propertyIndex,
	kodgen::MacroCodeGenEnv& env,
	std::string& out_Result) noexcept
{
	const kodgen::FieldInfo& field = reinterpret_cast<const kodgen::FieldInfo&>(entity);

	std::string strType = field.type.getCanonicalName();

	out_Result += strType + " Test() const;";
	out_Result += env.getSeparator();
	return true;
}

bool codegen::TestProperty::generateSourceFileHeaderCodeForEntity(
	const kodgen::EntityInfo& entity,
	const kodgen::Property& property,
	kodgen::uint8 propertyIndex,
	kodgen::MacroCodeGenEnv& env,
	std::string& out_Result) noexcept
{
	const kodgen::FieldInfo& field = reinterpret_cast<const kodgen::FieldInfo&>(entity);

	std::string strName = field.name;
	std::string strNamespace = entity.outerEntity->getFullName();
	std::string strType = field.type.getCanonicalName();

	out_Result += strType + " " + strNamespace + "::Test() const\n";
	out_Result += "{\n";
	out_Result += "\treturn " + strName + ";\n";
	out_Result += "}\n";
	out_Result += env.getSeparator();
	return true;
}