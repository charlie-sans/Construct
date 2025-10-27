#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "clang_compiler.h"
#include "file_includer.h"
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

void printUsage(const char* program) {
    std::cerr << "Usage: " << program << " <file.ct> [options]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -o <output>       Output file (default: <input>.ll for IR)" << std::endl;
    std::cerr << "  -c                Compile to object file (.o)" << std::endl;
    std::cerr << "  -x                Compile to executable" << std::endl;
    std::cerr << "  -S                Compile to assembly (.s)" << std::endl;
    std::cerr << "  -O<level>         Optimization level: 0, 1, 2, 3, s, z (default: 0)" << std::endl;
    std::cerr << "  -v                Verbose output (show generated IR)" << std::endl;
    std::cerr << "  --keep-temps      Keep temporary files" << std::endl;
    std::cerr << "  --list-stdlib     Print all stdlib functions as JSON and exit" << std::endl;
    std::cerr << "  --help            Show this help message" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Examples:" << std::endl;
    std::cerr << "  " << program << " main.ct                    # Compile to LLVM IR (main.ll)" << std::endl;
    std::cerr << "  " << program << " main.ct -c -o main.o       # Compile to object file" << std::endl;
    std::cerr << "  " << program << " main.ct -x -o main         # Compile to executable" << std::endl;
    std::cerr << "  " << program << " main.ct -S -o main.s       # Compile to assembly" << std::endl;
    std::cerr << "  " << program << " --list-stdlib              # Show stdlib functions (for IDE)" << std::endl;
}

int main(int argc, char* argv[]) {
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
            std::cerr << "Error: " << e.what() << std::endl;
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
    int optimize_level = 0;
    
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-c") {
            compile_to_object = true;
        } else if (arg == "-x") {
            compile_to_executable = true;
        } else if (arg == "-S") {
            compile_to_assembly = true;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "--keep-temps") {
            keep_temps = true;
        } else if (arg.substr(0, 2) == "-O") {
            if (arg.length() > 2) {
                optimize_level = std::stoi(arg.substr(2));
            }
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
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
        // Read source file
        std::cout << "Reading " << input_file << "..." << std::endl;
        std::string source = readFile(input_file);
        
        // Lex
        std::cout << "Tokenizing..." << std::endl;
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        std::cout << "  Generated " << tokens.size() << " tokens" << std::endl;
        
        // Parse
        std::cout << "Parsing..." << std::endl;
        Parser parser(tokens);
        Program program = parser.parse();
        std::cout << "  Parsed " << program.statements.size() << " statements" << std::endl;
        
        // Process include directives
        std::cout << "Processing includes..." << std::endl;
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
            std::cerr << "Warning: Failed to set include paths: " << e.what() << std::endl;
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
                    std::cout << "  Including " << files.size() << " file(s)" << std::endl;
                    
                    // Read and parse each included file
                    for (const auto& file : files) {
                        std::cout << "    - " << file << std::endl;
                        Program included = includer.readAndParseFile(file);
                        included_programs.push_back(included);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Include failed - " << e.what() << std::endl;
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
            std::cout << "  Merged " << included_programs.size() << " program(s) - total statements: " 
                      << final_program.statements.size() << std::endl;
        }
        
        // Compile to LLVM IR
        std::cout << "Compiling to LLVM IR..." << std::endl;
        Compiler compiler;
        std::string ir_code = compiler.compileToIR(final_program);
        
        // Print IR if verbose
        if (verbose) {
            std::cout << "\n=== Generated LLVM IR ===" << std::endl;
            std::cout << ir_code << std::endl;
            std::cout << "=== End IR ===" << std::endl << std::endl;
        }
        
        // Compile IR to desired output format
        if (compile_to_executable) {
            std::cout << "Compiling to executable..." << std::endl;
            ClangCompiler clang_compiler;
            clang_compiler.setKeepTemps(keep_temps);
            
            if (!clang_compiler.compileToExecutable(ir_code, output_file, {}, optimize_level)) {
                std::cerr << "Error: " << clang_compiler.getLastError() << std::endl;
                return 1;
            }
            
            std::cout << "Executable created: " << output_file << std::endl;
            
        } else if (compile_to_object) {
            std::cout << "Compiling to object file..." << std::endl;
            ClangCompiler clang_compiler;
            clang_compiler.setKeepTemps(keep_temps);
            
            if (!clang_compiler.compileToObjectFile(ir_code, output_file, optimize_level)) {
                std::cerr << "Error: " << clang_compiler.getLastError() << std::endl;
                return 1;
            }
            
            std::cout << "Object file created: " << output_file << std::endl;
            
        } else if (compile_to_assembly) {
            std::cout << "Compiling to assembly..." << std::endl;
            ClangCompiler clang_compiler;
            clang_compiler.setKeepTemps(keep_temps);
            
            if (!clang_compiler.compileToAssembly(ir_code, output_file, optimize_level)) {
                std::cerr << "Error: " << clang_compiler.getLastError() << std::endl;
                return 1;
            }
            
            std::cout << "Assembly file created: " << output_file << std::endl;
            
        } else {
            // Default: output LLVM IR
            std::cout << "Writing LLVM IR to " << output_file << "..." << std::endl;
            writeFile(output_file, ir_code);
            std::cout << "IR file created: " << output_file << std::endl;
        }
        
        std::cout << "\nCompilation successful!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
