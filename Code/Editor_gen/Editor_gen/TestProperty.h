#pragma once

#include <string>

#include <Kodgen/CodeGen/Macro/MacroPropertyCodeGen.h>

namespace codegen
{
	class TestProperty : public kodgen::MacroPropertyCodeGen
	{
	public:
		TestProperty() noexcept;

		bool preGenerateCodeForEntity(
			const kodgen::EntityInfo& entity, 
			const kodgen::Property& property, 
			kodgen::uint8 propertyIndex, 
			kodgen::MacroCodeGenEnv& env) noexcept override;

		bool generateClassFooterCodeForEntity(
			const kodgen::EntityInfo& entity,
			const kodgen::Property& property,
			kodgen::uint8 propertyIndex,
			kodgen::MacroCodeGenEnv& env, 
			std::string& out_Result) noexcept override;

		bool generateSourceFileHeaderCodeForEntity(
			const kodgen::EntityInfo& entity,
			const kodgen::Property& property,
			kodgen::uint8 propertyIndex,
			kodgen::MacroCodeGenEnv& env,
			std::string& out_Result) noexcept override;
	};
}