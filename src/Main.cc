#include "CompilationContext.h"
#include "Util.h"
#include "ImportResolver.h"
#include "TypeResolver.h"
#include "Error.h"

#include <iostream>

void Usage(const char* progName) {
  std::cout << "usage: " << progName << " [options] sourceFile [sourceFile ...]" << std::endl;
  std::cout << std::endl << "\tsourceFile must end in .java" << std::endl << std::endl; 
  std::cout << "Options: " << std::endl;
  std::cout << "\t--token-list     : prints the list of tokens" << std::endl;
  std::cout << "\t--parse-tree     : prints the parse tree" << std::endl;
  std::cout << "\t--ast            : prints the abstract syntax tree" << std::endl;
  std::cout << "\t--trace-lexer    : shows the steps of lexing DFA" << std::endl;
  std::cout << "\t--trace-parser   : shows the steps of shift reduce parser" << std::endl;
  std::cout << "\t--hierarchy      : shows the resolved methods of each class in the hierarchy" << std::endl;
  std::cout << "\t--help           : shows this message" << std::endl;
  exit(0);
}

void ProcessOptions(CompilationContext& context, int argc, char *argv[]) {
	// Add Standard Libraries
	context.AddSourceFile("stdlib/Int.sea");
	context.AddSourceFile("stdlib/Byte.sea");
	context.AddSourceFile("stdlib/Short.sea");
	context.AddSourceFile("stdlib/Long.sea");
	context.AddSourceFile("stdlib/Float.sea");
	context.AddSourceFile("stdlib/Double.sea");
	context.AddSourceFile("stdlib/Boolean.sea");
	context.AddSourceFile("stdlib/Char.sea");
	context.AddSourceFile("stdlib/String.sea");
	context.AddSourceFile("stdlib/Any.sea");

	for (int i = 1; i < argc; i++) {
		std::string arg { argv[i] };
		if (arg == "--token-list") {
			context.SetOption(SeaOption::PRINT_TOKENS);
		}
		else if (arg == "--ast") { 
			context.SetOption(SeaOption::PRINT_AST);
		}
		else if (arg == "--parse-tree") { 
			context.SetOption(SeaOption::PRINT_PARSE_TREE);
		}
		else if (arg == "--trace-lexer") { 
			context.SetOption(SeaOption::TRACE_DFA_LEXER);
		}
		else if (arg == "--trace-parser") { 
			context.SetOption(SeaOption::TRACE_SHIFT_REDUCE_PARSER);
		}
		else if (arg == "--hierarchy") { 
			context.SetOption(SeaOption::OUTPUT_HIERARCHY_RESOLUTION);
		}
		else if (arg == "--help") {
			Usage(argv[0]);
		}
		else {
			context.AddSourceFile(arg);
		}
	}
}
int main(int argc, char *argv[]) {
	CompilationContext context;
	try {
		ProcessOptions(context, argc, argv);

		// Resolves all the class / types known to each unit
		ImportResolver importResolver { context };

		TypeResolver typeResolver { context };

		// // Links Types to their Declaration Nodes
		// TypeLinker typeLinker(context);

		// // TODO: Move EnvironmentBuilder before TypeLinker?
		// EnvironmentBuilder environmentBuilder(context);

		// // Handles all the inheritance hierarchy
		// HierarchyChecker checker(context);

		// // Resolves all the names 
		// NameDisambiguator nameDisambiguator(context);

		// TypeChecker typeChecker(context);

		// ConstantFolder constantFolder(context);    

		// ReachabilityChecker reachabilityChecker(context);

		if (context.GetOption(SeaOption::PRINT_AST)) {
			context.DebugPrintAllAst(std::cout);
		}

		// // Assign ids to anything that needs it
		// IdAssigner idAssigner(context);

		// // Generate x86 code
		// CodeGenerator codeGenerator(context);

	} catch (SeaError &e) {
		LOG_ERROR(e.what());
		// Show source context
		context.PrintSourceForError(e.fileName, e.line, e.col);
		return 1;
	} catch (std::exception& e) {
		LOG_ERROR(e.what());
		return 1;
	} catch (const char* e) {
		LOG_WARN("The following error was thrown as a const char*!");
		LOG_ERROR(e);
		return 1;
	}
  return 0;
}
