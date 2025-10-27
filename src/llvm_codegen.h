#ifndef CONSTRUCT_LLVM_CODEGEN_H
#define CONSTRUCT_LLVM_CODEGEN_H

#include "ast.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace construct {

/**
 * LLVM Code Generator
 * Converts Construct AST to LLVM IR using the LLVM C++ API
 */
class LLVMCodegen {
public:
    LLVMCodegen();
    ~LLVMCodegen();
    
    // Generate LLVM IR from AST
    std::string generateIR(const Program& program);
    
    // Generate object file
    bool generateObjectFile(const Program& program, const std::string& filename);
    
    // Generate executable
    bool generateExecutable(const Program& program, const std::string& filename);

private:
    // Use void* to hide LLVM types from header
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

}  // namespace construct

#endif  // CONSTRUCT_LLVM_CODEGEN_H
