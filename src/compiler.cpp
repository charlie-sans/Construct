#include "compiler.h"
#include <sstream>
#include <stdexcept>
#include <cmath>

namespace construct {

Compiler::Compiler() : llvm_codegen(std::make_unique<LLVMCodegen>()) {}

std::string Compiler::compileToIR(const Program& program) {
    return llvm_codegen->generateIR(program);
}

bool Compiler::compileToObject(const Program& program, const std::string& filename) {
    return llvm_codegen->generateObjectFile(program, filename);
}

bool Compiler::compileToExecutable(const Program& program, const std::string& filename) {
    return llvm_codegen->generateExecutable(program, filename);
}

std::vector<StdlibFunction> Compiler::getStdlibFunctions() const {
    return {
        // ====== Output Functions ======
        {
            "dump",
            "void",
            {"value"},
            "Output a value to stdout (works with int, float, bool, or string)"
        },
        {
            "dump_line",
            "void",
            {"value"},
            "Output a value to stdout with newline (works with int, float, bool, or string)"
        },
        {
            "print",
            "void",
            {"fmt"},
            "Print a formatted string"
        },
        {
            "nl",
            "void",
            {},
            "Print a newline character"
        },
        {
            "print_error",
            "void",
            {"msg"},
            "Print an error message to stderr"
        },
        {
            "debug",
            "void",
            {"msg"},
            "Print a debug message to stderr"
        },
        
        // ====== Input Functions ======
        {
            "read_int",
            "int",
            {},
            "Read an integer from stdin"
        },
        {
            "read_float",
            "float",
            {},
            "Read a floating-point number from stdin"
        },
        {
            "read_string",
            "string",
            {},
            "Read a word (non-whitespace) from stdin"
        },
        {
            "read_line",
            "string",
            {},
            "Read a full line (including spaces) from stdin"
        },
        {
            "read_line_prompt",
            "string",
            {"prompt"},
            "Read a line with a prompt message displayed first"
        },
        
        // ====== String Functions ======
        {
            "len",
            "int",
            {"s"},
            "Get the length of a string"
        },
        {
            "concat",
            "string",
            {"a", "b"},
            "Concatenate two strings"
        },
        {
            "eq",
            "bool",
            {"a", "b"},
            "Compare two strings for equality"
        },
        {
            "upper",
            "string",
            {"s"},
            "Convert string to uppercase"
        },
        {
            "lower",
            "string",
            {"s"},
            "Convert string to lowercase"
        },
        {
            "trim",
            "string",
            {"s"},
            "Trim whitespace from both ends of string"
        },
        {
            "starts_with",
            "bool",
            {"s", "prefix"},
            "Check if string starts with prefix"
        },
        {
            "ends_with",
            "bool",
            {"s", "suffix"},
            "Check if string ends with suffix"
        },
        
        // ====== Type Conversion ======
        {
            "toStr",
            "string",
            {"value"},
            "Convert a value to a string (works with int, float, or bool)"
        },
        {
            "toInt",
            "int",
            {"s"},
            "Convert a string to an integer"
        },
        {
            "toFloat",
            "float",
            {"s"},
            "Convert a string to a floating-point number"
        },
        {
            "toBool",
            "bool",
            {"s"},
            "Convert a string to a boolean"
        },
        
        // ====== Math Functions ======
        {
            "abs",
            "number",
            {"x"},
            "Get the absolute value of a number"
        },
        {
            "max",
            "number",
            {"a", "b"},
            "Return the maximum of two numbers"
        },
        {
            "min",
            "number",
            {"a", "b"},
            "Return the minimum of two numbers"
        },
        {
            "clamp",
            "number",
            {"val", "min", "max"},
            "Clamp a number between min and max"
        },
        {
            "round",
            "float",
            {"x"},
            "Round a number to the nearest integer"
        },
        {
            "floor",
            "float",
            {"x"},
            "Round down to the nearest integer"
        },
        {
            "ceil",
            "float",
            {"x"},
            "Round up to the nearest integer"
        },
        
        // ====== Utility Functions ======
        {
            "sleep",
            "void",
            {"seconds"},
            "Sleep for a number of seconds"
        },
        {
            "sleep_ms",
            "void",
            {"milliseconds"},
            "Sleep for a number of milliseconds"
        },
        {
            "clear_screen",
            "void",
            {},
            "Clear the terminal screen"
        },
        {
            "exit",
            "void",
            {"code"},
            "Exit the program with a status code"
        }
    };
}

std::string Compiler::getStdlibAsJSON() const {
    auto funcs = getStdlibFunctions();
    std::stringstream ss;
    
    ss << "{\n";
    ss << "  \"version\": \"0.1.0\",\n";
    ss << "  \"stdlib_functions\": [\n";
    
    for (size_t i = 0; i < funcs.size(); ++i) {
        const auto& func = funcs[i];
        ss << "    {\n";
        ss << "      \"name\": \"" << func.name << "\",\n";
        ss << "      \"return_type\": \"" << func.returnType << "\",\n";
        ss << "      \"parameters\": [";
        
        for (size_t j = 0; j < func.parameters.size(); ++j) {
            ss << "\"" << func.parameters[j] << "\"";
            if (j < func.parameters.size() - 1) ss << ", ";
        }
        
        ss << "],\n";
        ss << "      \"documentation\": \"" << func.documentation << "\"\n";
        ss << "    }";
        
        if (i < funcs.size() - 1) ss << ",";
        ss << "\n";
    }
    
    ss << "  ]\n";
    ss << "}\n";
    
    return ss.str();
}

std::vector<std::string> Compiler::getImportedModules() const {
    return llvm_codegen->getImportedModules();
}

}  // namespace construct
