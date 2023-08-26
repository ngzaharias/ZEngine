#include "Editor_gen/TestModule.h"

codegen::TestModule::TestModule() noexcept
{
	addPropertyCodeGen(m_TestProperty);
}

codegen::TestModule::TestModule(TestModule const&)
	: TestModule() // call the default constructor to add the copied instance its own property references
{
}

codegen::TestModule* codegen::TestModule::clone() const noexcept
{
	return new TestModule(*this);
}

bool codegen::TestModule::initialGenerateHeaderFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept
{
	if (!base::initialGenerateHeaderFileHeaderCode(env, out_Result))
		return false;

	return true;
}

bool codegen::TestModule::initialGenerateHeaderFileFooterCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept
{
	if (!base::initialGenerateHeaderFileFooterCode(env, out_Result))
		return false;

	return true;
}

bool codegen::TestModule::initialGenerateSourceFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept
{
	if (!base::initialGenerateSourceFileHeaderCode(env, out_Result))
		return false;

	return true;
}

bool codegen::TestModule::finalGenerateHeaderFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept
{
	if (!base::finalGenerateHeaderFileHeaderCode(env, out_Result))
		return false;

	return true;
}

bool codegen::TestModule::finalGenerateHeaderFileFooterCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept
{
	if (!base::finalGenerateHeaderFileFooterCode(env, out_Result))
		return false;

	return true;
}

bool codegen::TestModule::finalGenerateSourceFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept
{
	if (!base::finalGenerateSourceFileHeaderCode(env, out_Result))
		return false;

	return true;
}
