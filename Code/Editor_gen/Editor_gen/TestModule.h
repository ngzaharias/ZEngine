#pragma once

#include <Kodgen/CodeGen/Macro/MacroCodeGenModule.h>

#include "Editor_gen/TestProperty.h"

namespace codegen
{
	class TestModule : public kodgen::MacroCodeGenModule
	{
		using base = kodgen::MacroCodeGenModule;

	public:
		TestModule() noexcept;
		TestModule(TestModule const&);

		TestModule* clone() const noexcept override;

		bool initialGenerateHeaderFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept override;
		bool initialGenerateHeaderFileFooterCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept override;
		bool initialGenerateSourceFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept override;

		bool finalGenerateHeaderFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept override;
		bool finalGenerateHeaderFileFooterCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept override;
		bool finalGenerateSourceFileHeaderCode(kodgen::MacroCodeGenEnv& env, std::string& out_Result) noexcept override;

	private:
		TestProperty m_TestProperty;
	};
}