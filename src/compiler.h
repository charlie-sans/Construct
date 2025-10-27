#ifndef CONSTRUCT_COMPILER_H
#define CONSTRUCT_COMPILER_H

#include "ast.h"
#include "llvm_codegen.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace construct {

struct StdlibFunction {
    std::string name;
    std::string returnType;
    std::vector<std::string> parameters;
    std::string documentation;
};

class Compiler {
public:
    Compiler();
    
    // Generate LLVM IR using LLVM library
    std::string compileToIR(const Program& program);
    
    // Generate object file
    bool compileToObject(const Program& program, const std::string& filename);
    
    // Generate executable
    bool compileToExecutable(const Program& program, const std::string& filename);
    
    // Get all available stdlib functions as JSON
    std::string getStdlibAsJSON() const;
    
    // Get all available stdlib functions
    std::vector<StdlibFunction> getStdlibFunctions() const;
    
    // Get list of imported modules from the compiled program
    std::vector<std::string> getImportedModules() const;
    
private:
    std::unique_ptr<LLVMCodegen> llvm_codegen;
    
    // Type checking
    TypePtr inferType(const ExprPtr& expr);
    bool typesCompatible(const TypePtr& a, const TypePtr& b);
    
    // Helpers
    std::string typeToString(const TypePtr& type);
};

}  // namespace construct

#endif // CONSTRUCT_COMPILER_H
