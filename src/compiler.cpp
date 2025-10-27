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
        // Output Functions
        {
            "dump",
            "void",
            {"value"},
            "Output a value to stdout (works with int, float, bool, or string)"
        },
        {
            "print",
            "void",
            {"fmt", "..."},
            "Print a formatted string (printf-style)"
        },
        {
            "nl",
            "void",
            {},
            "Print a newline character"
        },
        
        // String Functions
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
            "substr",
            "string",
            {"s", "start", "length"},
            "Extract a substring"
        },
        {
            "indexOf",
            "int",
            {"s", "char"},
            "Find the index of a character in a string (returns -1 if not found)"
        },
        
        // Type Conversion Functions
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
        
        // Math Functions
        {
            "abs",
            "number",
            {"x"},
            "Get the absolute value of a number"
        },
        {
            "sqrt",
            "float",
            {"x"},
            "Get the square root of a number"
        },
        {
            "pow",
            "number",
            {"base", "exp"},
            "Raise base to the power of exp"
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
