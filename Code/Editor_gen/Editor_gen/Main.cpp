#include <Kodgen/Parsing/FileParser.h>
#include <Kodgen/CodeGen/CodeGenManager.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnit.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnitSettings.h>
#include <Kodgen/Misc/Filesystem.h>
#include <Kodgen/Misc/DefaultLogger.h>

#include "Editor_gen/TestModule.h"

bool initParsingSettings(kodgen::ParsingSettings& parsingSettings)
{
	parsingSettings.shouldAbortParsingOnFirstError = true;
	parsingSettings.shouldParseAllNamespaces = true;

	parsingSettings.propertyParsingSettings.propertySeparator = ',';
	parsingSettings.propertyParsingSettings.argumentEnclosers[0] = '(';
	parsingSettings.propertyParsingSettings.argumentEnclosers[1] = ')';
	parsingSettings.propertyParsingSettings.argumentSeparator = ',';

	parsingSettings.propertyParsingSettings.namespaceMacroName = "ZNamespace";
	parsingSettings.propertyParsingSettings.classMacroName = "ZClass";
	parsingSettings.propertyParsingSettings.structMacroName = "ZStruct";
	parsingSettings.propertyParsingSettings.fieldMacroName = "ZVariable";
	parsingSettings.propertyParsingSettings.fieldMacroName = "ZField";
	parsingSettings.propertyParsingSettings.functionMacroName = "ZFunction";
	parsingSettings.propertyParsingSettings.methodMacroName = "ZMethod";
	parsingSettings.propertyParsingSettings.enumMacroName = "ZEnum";
	parsingSettings.propertyParsingSettings.enumValueMacroName = "ZEnumVal";

	// This is setup that way for CI tools only
	// In reality, the compiler used by the user machine running the generator should be set.
	// It has nothing to see with the compiler used to compile the generator.
#if defined(__GNUC__)
	return parsingSettings.setCompilerExeName("g++");
#elif defined(__clang__)
	return parsingSettings.setCompilerExeName("clang++");
#elif defined(_MSC_VER)
	return parsingSettings.setCompilerExeName("msvc");
#else
	return false;	// Unsupported compiler
#endif
}

int main(int argc, char** argv)
{
	kodgen::DefaultLogger logger;

	if (argc <= 2)
	{
		logger.log("No working directory provided as first program argument", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	fs::path workingDirectory = argv[1];
	fs::path generatedDirectory = argv[2];

	if (!fs::is_directory(workingDirectory))
	{
		logger.log("Provided working directory is not a directory or doesn't exist", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	logger.log("Working Directory: " + workingDirectory.string());

	// Setup FileParser
	kodgen::FileParser fileParser;
	fileParser.logger = &logger;

	if (!initParsingSettings(fileParser.getSettings()))
	{
		logger.log("Compiler could not be set because it is not supported on the current machine or vswhere could not be found (Windows|MSVC only).", kodgen::ILogger::ELogSeverity::Error);
		return EXIT_FAILURE;
	}

	// Setup code generation unit
	kodgen::MacroCodeGenUnit codeGenUnit;
	codeGenUnit.logger = &logger;

	kodgen::MacroCodeGenUnitSettings cguSettings;
	cguSettings.setOutputDirectory(generatedDirectory);
	cguSettings.setGeneratedHeaderFileNamePattern("##FILENAME##_gen.h");
	cguSettings.setGeneratedSourceFileNamePattern("##FILENAME##_gen.cpp.h");
	cguSettings.setClassFooterMacroPattern("##CLASSNAME##_GENERATED");
	cguSettings.setHeaderFileFooterMacroPattern("##FILENAME##_GENERATED");
	codeGenUnit.setSettings(cguSettings);

	// Add code generation modules
	codegen::TestModule testModule;
	codeGenUnit.addModule(testModule);

	// Setup CodeGenManager
	kodgen::CodeGenManager codeGenMgr;
	codeGenMgr.logger = &logger;

	codeGenMgr.settings.addToProcessDirectory(workingDirectory);
	codeGenMgr.settings.addIgnoredDirectory(generatedDirectory);
	codeGenMgr.settings.addSupportedFileExtension(".h");

	// Kick-off code generation
	kodgen::CodeGenResult genResult = codeGenMgr.run(fileParser, codeGenUnit, true);

	if (genResult.completed)
	{
		logger.log("Generation completed successfully in " + std::to_string(genResult.duration) + " seconds.");
	}
	else
	{
		logger.log("An error happened during code generation.", kodgen::ILogger::ELogSeverity::Error);
	}

	system("pause");
	return EXIT_SUCCESS;
}