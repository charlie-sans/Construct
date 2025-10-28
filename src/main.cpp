#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "clang_compiler.h"
#include "file_includer.h"
#include "ast_printer.h"
#include "diagnostics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace construct;

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not write to file: " + filename);
    }
    file << content;
}

// Check if NO_COLOR environment variable is set
void initializeColorSupport() {
    const char* no_color = std::getenv("NO_COLOR");
    if (no_color != nullptr) {
        TerminalStyle::setColorEnabled(false);
    }
}

void printUsage(const char* program) {
    std::cerr << "Usage: " << program << " <file.ct> [options]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -o <output>       Output file (default: <input>.ll for IR)" << std::endl;
    std::cerr << "  -c                Compile to object file (.o)" << std::endl;
    std::cerr << "  -x                Compile to executable" << std::endl;
    std::cerr << "  -S                Compile to assembly (.s)" << std::endl;
    std::cerr << "  -l <lib>          Link with library (e.g., -l raylib)" << std::endl;
    std::cerr << "  -O<level>         Optimization level: 0, 1, 2, 3, s, z (default: 0)" << std::endl;
    std::cerr << "  -v                Verbose output (show generated IR)" << std::endl;
    std::cerr << "  --dump-ast        Print the AST and exit" << std::endl;
    std::cerr << "  --keep-temps      Keep temporary files" << std::endl;
    std::cerr << "  --list-stdlib     Print all stdlib functions as JSON and exit" << std::endl;
    std::cerr << "  --help            Show this help message" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "  " << program << " main.ct                    # Compile to LLVM IR (main.ll)" << std::endl;
    std::cerr << "  " << program << " main.ct -c -o main.o       # Compile to object file" << std::endl;
    std::cerr << "  " << program << " main.ct -x -o main         # Compile to executable" << std::endl;
    std::cerr << "  " << program << " main.ct -S -o main.s       # Compile to assembly" << std::endl;
    std::cerr << "  " << program << " main.ct --dump-ast         # Print the AST" << std::endl;
    std::cerr << "  " << program << " --list-stdlib              # Show stdlib functions (for IDE)" << std::endl;
}

int main(int argc, char* argv[]) {
    initializeColorSupport();
    
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    // Check for list-stdlib flag early
    if (std::string(argv[1]) == "--list-stdlib") {
        try {
            Compiler compiler;
            std::cout << compiler.getStdlibAsJSON();
            return 0;
        } catch (const std::exception& e) {
            Diagnostics::error(e.what());
            return 1;
        }
    }
    
    std::string input_file = argv[1];
    
    // Check for help flag
    if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        printUsage(argv[0]);
        return 0;
    }
    
    // Parse command line arguments
    std::string output_file;
    bool compile_to_object = false;
    bool compile_to_executable = false;
    bool compile_to_assembly = false;
    bool verbose = false;
    bool keep_temps = false;
    bool dump_ast = false;
    int optimize_level = 0;
    std::vector<std::string> libraries;  // Libraries to link
    
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-l" && i + 1 < argc) {
            libraries.push_back(argv[++i]);
        } else if (arg == "-c") {
            compile_to_object = true;
        } else if (arg == "-x") {
            compile_to_executable = true;
        } else if (arg == "-S") {
            compile_to_assembly = true;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "--dump-ast") {
            dump_ast = true;
        } else if (arg == "--keep-temps") {
            keep_temps = true;
        } else if (arg.substr(0, 2) == "-O") {
            if (arg.length() > 2) {
                optimize_level = std::stoi(arg.substr(2));
            }
        } else {
            Diagnostics::error("Unknown option: " + arg);
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Determine output file if not specified
    if (output_file.empty()) {
        std::string stem = fs::path(input_file).stem().string();
        
        if (compile_to_object) {
            output_file = stem + ".o";
        } else if (compile_to_executable) {
            output_file = stem;
        } else if (compile_to_assembly) {
            output_file = stem + ".s";
        } else {
            output_file = stem + ".ll";
        }
    }
    
    try {
        ProgressIndicator progress("Construct Compiler", 6);
        
        // Read source file
        Diagnostics::step("Reading", input_file);
        std::string source = readFile(input_file);
        progress.completeStep("Reading source file");
        
        // Lex
        Diagnostics::step("Tokenizing", "source code");
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        Diagnostics::stat("tokens", tokens.size());
        progress.completeStep("Tokenizing", std::to_string(tokens.size()) + " tokens");
        
        // Parse
        Diagnostics::step("Parsing", "AST generation");
        Parser parser(tokens);
        Program program = parser.parse();
        Diagnostics::stat("statements", program.statements.size());
        progress.completeStep("Parsing", std::to_string(program.statements.size()) + " statements");
        
        // Dump AST if requested
        if (dump_ast) {
            Diagnostics::header("Abstract Syntax Tree");
            ASTPrinter::printProgram(program);
            return 0;
        }
        
        // Process include directives
        Diagnostics::step("Processing", "include directives");
        FileIncluder includer;
        
        // Configure include paths
        std::string input_dir = fs::path(input_file).parent_path().string();
        if (input_dir.empty()) input_dir = ".";
        
        // Set up include path search order
        std::vector<std::string> include_paths;
        
        // First, add the directory where the compiler executable is located
        std::string compiler_dir = fs::path(argv[0]).parent_path().string();
        if (compiler_dir.empty()) compiler_dir = ".";
        include_paths.push_back(compiler_dir);                 // Compiler's directory
        
        include_paths.push_back(input_dir);                    // Current file's directory
        include_paths.push_back(fs::path(input_dir) / "lib");  // lib subdirectory
        include_paths.push_back(".");                          // Current working directory
        
        try {
            includer.setIncludePaths(include_paths);
        } catch (const std::exception& e) {
            Diagnostics::warning(std::string("Failed to set include paths: ") + e.what());
        }
        
        Program final_program = program;
        
        // Find and process all INCLUDE statements
        std::vector<Program> included_programs;
        std::vector<StmtPtr> processed_stmts;
        
        for (const auto& stmt : program.statements) {
            if (stmt->kind == Statement::INCLUDE) {
                try {
                    // Get base directory from input file
                    std::string base_dir = fs::path(input_file).parent_path().string();
                    if (base_dir.empty()) base_dir = ".";
                    
                    // Resolve include paths
                    auto files = includer.resolveFiles(stmt->include_paths, base_dir);
                    Diagnostics::stat("included files", files.size());
                    
                    // Read and parse each included file
                    for (const auto& file : files) {
                        Program included = includer.readAndParseFile(file);
                        included_programs.push_back(included);
                    }
                } catch (const std::exception& e) {
                    Diagnostics::warning(std::string("Include failed: ") + e.what());
                }
            } else {
                // Keep non-include statements
                processed_stmts.push_back(stmt);
            }
        }
        
        // Merge included programs with the main program
        if (!included_programs.empty()) {
            final_program.statements = processed_stmts;
            final_program = includer.mergePrograms(final_program, included_programs);
            Diagnostics::stat("merged programs", included_programs.size());
            Diagnostics::stat("total statements", final_program.statements.size());
        }
        progress.completeStep("Processing includes");
        
        // Compile to LLVM IR
        Diagnostics::step("Compiling", "to LLVM IR");
        Compiler compiler;
        std::string ir_code = compiler.compileToIR(final_program);
        progress.completeStep("LLVM IR generation");
        
        // Print IR if verbose
        if (verbose) {
            Diagnostics::header("Generated LLVM IR");
            std::cout << ir_code << std::endl;
            Diagnostics::separator();
        }
        
        // Compile IR to desired output format
        if (compile_to_executable) {
            Diagnostics::step("Linking", "to executable");
            ClangCompiler clang_compiler(argv[0]);
            clang_compiler.setKeepTemps(keep_temps);
            
            if (!clang_compiler.compileToExecutable(ir_code, output_file, {}, optimize_level, libraries)) {
                progress.failed(clang_compiler.getLastError());
                return 1;
            }
            
            progress.completeStep("Executable generation", output_file);
            
        } else if (compile_to_object) {
            Diagnostics::step("Compiling", "to object file");
            ClangCompiler clang_compiler(argv[0]);
            clang_compiler.setKeepTemps(keep_temps);
            
            if (!clang_compiler.compileToObjectFile(ir_code, output_file, optimize_level)) {
                progress.failed(clang_compiler.getLastError());
                return 1;
            }
            
            progress.completeStep("Object file generation", output_file);
            
        } else if (compile_to_assembly) {
            Diagnostics::step("Compiling", "to assembly");
            ClangCompiler clang_compiler(argv[0]);
            clang_compiler.setKeepTemps(keep_temps);
            
            if (!clang_compiler.compileToAssembly(ir_code, output_file, optimize_level)) {
                progress.failed(clang_compiler.getLastError());
                return 1;
            }
            
            progress.completeStep("Assembly generation", output_file);
            
        } else {
            // Default: output LLVM IR
            Diagnostics::step("Writing", "LLVM IR to disk");
            writeFile(output_file, ir_code);
            progress.completeStep("IR file generation", output_file);
        }
        
        progress.complete();
        return 0;
        
    } catch (const std::exception& e) {
        Diagnostics::error(e.what());
        return 1;
    }
}
