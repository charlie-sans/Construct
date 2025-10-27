#include "llvm_codegen.h"
#include <stdexcept>
#include <sstream>

// LLVM includes
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Intrinsics.h"

namespace construct {

using namespace llvm;

class LLVMCodegen::Impl {
public:
    Impl();
    ~Impl();
    
    std::string generateIR(const Program& program);
    bool generateObjectFile(const Program& program, const std::string& filename);
    bool generateExecutable(const Program& program, const std::string& filename);
    
    // Get list of modules that were imported
    const std::vector<std::string>& getImportedModules() const { return imported_modules; }

private:
    std::unique_ptr<LLVMContext> context;
    std::unique_ptr<Module> module;
    std::unique_ptr<IRBuilder<>> builder;
    
    // Symbol table for type information
    std::unordered_map<std::string, Value*> symbol_table;
    std::unordered_map<std::string, llvm::Type*> type_table;
    
    // Track imported modules for linking
    std::vector<std::string> imported_modules;
    
    // Helper methods
    llvm::Type* convertType(const TypePtr& type);
    Value* codegenExpr(const ExprPtr& expr);
    void codegenStmt(const StmtPtr& stmt);
    Function* codegenFunction(const ExprPtr& func_expr, const std::string& name);
    
    // Expression code generation
    Value* codegenBinaryOp(const ExprPtr& expr);
    Value* codegenUnaryOp(const ExprPtr& expr);
    Value* codegenIfExpr(const ExprPtr& expr);
    Value* codegenMatchExpr(const ExprPtr& expr);
    Value* codegenForLoop(const ExprPtr& expr);
    Value* codegenWhileLoop(const ExprPtr& expr);
    Value* codegenCallExpr(const ExprPtr& expr);
    Value* codegenListLiteral(const ExprPtr& expr);
    Value* codegenRecordLiteral(const ExprPtr& expr);
    
    // Utility
    void initBuiltins();
    std::string moduleToString();
};

LLVMCodegen::Impl::Impl() {
    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("construct", *context);
    builder = std::make_unique<IRBuilder<>>(*context);
    initBuiltins();
}

LLVMCodegen::Impl::~Impl() = default;

void LLVMCodegen::Impl::initBuiltins() {
    // Add standard library functions
    // printf(fmt, ...) - for general formatting
    std::vector<llvm::Type*> printf_args = {
        llvm::PointerType::getUnqual(*context)
    };
    FunctionType* printf_type = FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        printf_args,
        true  // variadic
    );
    Function::Create(printf_type, Function::ExternalLinkage, "printf", module.get());
    
    // puts(str) - for simple string output
    std::vector<llvm::Type*> puts_args = {llvm::PointerType::getUnqual(*context)};
    FunctionType* puts_type = FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        puts_args,
        false
    );
    Function::Create(puts_type, Function::ExternalLinkage, "puts", module.get());
    
    // fdump(const char* format, const char* value) - for formatted dumping
    std::vector<llvm::Type*> fdump_args = {
        llvm::PointerType::getUnqual(*context),
        llvm::PointerType::getUnqual(*context)
    };
    FunctionType* fdump_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        fdump_args,
        false
    );
    Function::Create(fdump_type, Function::ExternalLinkage, "fdump", module.get());

    // --- glibc functions ---
    // malloc(size_t)
    FunctionType* malloc_type = FunctionType::get(
        llvm::PointerType::getUnqual(*context),
        {llvm::Type::getInt64Ty(*context)},
        false
    );
    Function::Create(malloc_type, Function::ExternalLinkage, "malloc", module.get());

    // free(void*)
    FunctionType* free_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        {llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(free_type, Function::ExternalLinkage, "free", module.get());

    // strlen(const char*)
    FunctionType* strlen_type = FunctionType::get(
        llvm::Type::getInt64Ty(*context),
        {llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(strlen_type, Function::ExternalLinkage, "strlen", module.get());

    // strcpy(char*, const char*)
    FunctionType* strcpy_type = FunctionType::get(
        llvm::PointerType::getUnqual(*context),
        {llvm::PointerType::getUnqual(*context), llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(strcpy_type, Function::ExternalLinkage, "strcpy", module.get());

    // strcat(char*, const char*)
    FunctionType* strcat_type = FunctionType::get(
        llvm::PointerType::getUnqual(*context),
        {llvm::PointerType::getUnqual(*context), llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(strcat_type, Function::ExternalLinkage, "strcat", module.get());

    // strcmp(const char*, const char*)
    FunctionType* strcmp_type = FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::getUnqual(*context), llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(strcmp_type, Function::ExternalLinkage, "strcmp", module.get());

    // exit(int)
    FunctionType* exit_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        {llvm::Type::getInt32Ty(*context)},
        false
    );
    Function::Create(exit_type, Function::ExternalLinkage, "exit", module.get());

    // getenv(const char*)
    FunctionType* getenv_type = FunctionType::get(
        llvm::PointerType::getUnqual(*context),
        {llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(getenv_type, Function::ExternalLinkage, "getenv", module.get());

    // system(const char*)
    FunctionType* system_type = FunctionType::get(
        llvm::Type::getInt32Ty(*context),
        {llvm::PointerType::getUnqual(*context)},
        false
    );
    Function::Create(system_type, Function::ExternalLinkage, "system", module.get());
    
    // dump_int(n) - for dumping integers
    std::vector<llvm::Type*> dump_int_args = {llvm::Type::getInt32Ty(*context)};
    FunctionType* dump_int_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        dump_int_args,
        false
    );
    Function::Create(dump_int_type, Function::ExternalLinkage, "construct_dump_int", module.get());
    
    // dump_float(f) - for dumping floats
    std::vector<llvm::Type*> dump_float_args = {llvm::Type::getDoubleTy(*context)};
    FunctionType* dump_float_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        dump_float_args,
        false
    );
    Function::Create(dump_float_type, Function::ExternalLinkage, "construct_dump_float", module.get());
    
    // dump_bool(b) - for dumping booleans
    std::vector<llvm::Type*> dump_bool_args = {llvm::Type::getInt1Ty(*context)};
    FunctionType* dump_bool_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        dump_bool_args,
        false
    );
    Function::Create(dump_bool_type, Function::ExternalLinkage, "construct_dump_bool", module.get());
    
    // dump_string(s) - for dumping strings
    std::vector<llvm::Type*> dump_string_args = {llvm::PointerType::getUnqual(*context)};
    FunctionType* dump_string_type = FunctionType::get(
        llvm::Type::getVoidTy(*context),
        dump_string_args,
        false
    );
    Function::Create(dump_string_type, Function::ExternalLinkage, "construct_dump_string", module.get());
}

llvm::Type* LLVMCodegen::Impl::convertType(const TypePtr& type) {
    if (!type) return llvm::Type::getInt32Ty(*context);
    
    switch (type->kind) {
        case Type::INT:
            return llvm::Type::getInt32Ty(*context);
            
        case Type::FLOAT:
            return llvm::Type::getDoubleTy(*context);
            
        case Type::BOOL:
            return llvm::Type::getInt1Ty(*context);
            
        case Type::STRING:
            return llvm::PointerType::getUnqual(*context);
            
        case Type::LIST: {
            // For now, represent as i32* (array of i32)
            return llvm::PointerType::get(llvm::Type::getInt32Ty(*context), 0);
        }
            
        case Type::TUPLE: {
            std::vector<llvm::Type*> element_types;
            for (const auto& elem_type : type->element_types) {
                element_types.push_back(convertType(elem_type));
            }
            return StructType::create(*context, element_types);
        }
            
        case Type::RECORD: {
            std::vector<llvm::Type*> field_types;
            for (const auto& [name, field_type] : type->fields) {
                field_types.push_back(convertType(field_type));
            }
            return StructType::create(*context, field_types);
        }
            
        case Type::FUNCTION: {
            auto param_type = convertType(type->param_type);
            auto return_type = convertType(type->return_type);
            return FunctionType::get(return_type, {param_type}, false);
        }
            
        default:
            return llvm::Type::getInt32Ty(*context);
    }
}

std::string LLVMCodegen::Impl::generateIR(const Program& program) {
    // Reset symbol table for new module
    symbol_table.clear();
    type_table.clear();
    
    // Create main function first
    FunctionType* main_type = FunctionType::get(llvm::Type::getInt32Ty(*context), false);
    Function* main_fn = Function::Create(
        main_type,
        Function::ExternalLinkage,
        "main",
        module.get()
    );
    BasicBlock* entry = BasicBlock::Create(*context, "entry", main_fn);
    builder->SetInsertPoint(entry);
    
    // Process statements and track the last value
    Value* last_value = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
    for (const auto& stmt : program.statements) {
        try {
            if (stmt->kind == Statement::EXPR_STMT) {
                // Check if this is a function definition (LAMBDA with fn_name)
                if (stmt->expr && stmt->expr->kind == Expr::LAMBDA && !stmt->expr->fn_name.empty()) {
                    auto fn = codegenFunction(stmt->expr, stmt->expr->fn_name);
                    if (fn) {
                        symbol_table[stmt->expr->fn_name] = fn;
                        last_value = fn;
                    }
                } else {
                    auto val = codegenExpr(stmt->expr);
                    // Only update last_value if we get a non-void result
                    if (val && !val->getType()->isVoidTy()) {
                        last_value = val;
                    }
                }
            } else if (stmt->kind == Statement::LET_BINDING) {
                // Check if binding is a function definition
                if (stmt->expr && stmt->expr->kind == Expr::LAMBDA) {
                    // Compile as function
                    auto fn = codegenFunction(stmt->expr, stmt->name);
                    if (fn) {
                        symbol_table[stmt->name] = fn;
                        last_value = fn;
                    }
                } else {
                    auto value = codegenExpr(stmt->expr);
                    if (value) {
                        symbol_table[stmt->name] = value;
                        last_value = value;
                    }
                }
            } else {
                codegenStmt(stmt);
            }
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Codegen error: ") + e.what());
        }
    }
    
    // Ensure we're in a valid basic block
    BasicBlock* current_block = builder->GetInsertBlock();
    if (!current_block) {
        // This shouldn't happen, but if it does, we need to recover
        throw std::runtime_error("No insert point after generating statements");
    }
    
    // Convert result to i32 if needed (for boolean and other types)
    if (last_value->getType() != llvm::Type::getInt32Ty(*context)) {
        if (last_value->getType()->isIntegerTy(1)) {
            // Convert i1 to i32: i1 true becomes 1, i1 false becomes 0
            last_value = builder->CreateZExt(last_value, llvm::Type::getInt32Ty(*context));
        } else if (last_value->getType()->isPointerTy() || last_value->getType()->isFunctionTy()) {
            // For pointers or functions, just use 0
            last_value = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
        }
    }
    
    // Add return statement to the current block if it doesn't already have a terminator
    if (!current_block->getTerminator()) {
        builder->CreateRet(last_value);
    }
    // If the current block already has a terminator (e.g., from a branch),
    // we need to make sure the last value is properly returned in all paths.
    // This is a simplified approach - a more sophisticated compiler would
    // need to ensure all code paths have returns.
    
    // Verify module
    std::string error;
    raw_string_ostream error_stream(error);
    if (verifyModule(*module, &error_stream)) {
        // Print the IR even if verification fails for debugging
        llvm::errs() << "Module verification failed: " << error << "\n";
        llvm::errs() << "\n=== Generated IR (for debugging) ===\n";
        module->print(llvm::errs(), nullptr);
        llvm::errs() << "\n===================================\n";
        throw std::runtime_error("Module verification failed");
    }
    
    return moduleToString();
}

std::string LLVMCodegen::Impl::moduleToString() {
    std::string ir_str;
    raw_string_ostream os(ir_str);
    module->print(os, nullptr);
    os.flush();
    return ir_str;
}

void LLVMCodegen::Impl::codegenStmt(const StmtPtr& stmt) {
    if (!stmt) return;
    
    switch (stmt->kind) {
        case Statement::EXPR_STMT:
            codegenExpr(stmt->expr);
            break;
            
        case Statement::LET_BINDING: {
            auto value = codegenExpr(stmt->expr);
            if (value) {
                symbol_table[stmt->name] = value;
            }
            break;
        }
            
        case Statement::FUNCTION_DEF:
            if (stmt->expr && stmt->expr->kind == Expr::LAMBDA) {
                codegenFunction(stmt->expr, stmt->name);
            }
            break;
        
        case Statement::IMPORT:
            // Record imported module for linking
            imported_modules.push_back(stmt->module_name);
            break;
            
        default:
            break;
    }
}

Value* LLVMCodegen::Impl::codegenExpr(const ExprPtr& expr) {
    if (!expr) return nullptr;
    
    switch (expr->kind) {
        case Expr::INT_LITERAL: {
            int value = std::stoi(expr->literal_value);
            return ConstantInt::get(llvm::Type::getInt32Ty(*context), value);
        }
            
        case Expr::FLOAT_LITERAL: {
            double value = std::stod(expr->literal_value);
            return ConstantFP::get(llvm::Type::getDoubleTy(*context), value);
        }
            
        case Expr::STRING_LITERAL: {
            return builder->CreateGlobalString(expr->literal_value, "str");
        }
            
        case Expr::BOOL_LITERAL: {
            bool value = expr->literal_value == "true";
            return ConstantInt::get(llvm::Type::getInt1Ty(*context), value);
        }
            
        case Expr::IDENTIFIER: {
            auto it = symbol_table.find(expr->name);
            if (it != symbol_table.end()) {
                return it->second;
            }
            // Try to find as function
            if (auto fn = module->getFunction(expr->name)) {
                return fn;
            }
            throw std::runtime_error("Unknown identifier: " + expr->name);
        }
            
        case Expr::BINARY_OP:
            return codegenBinaryOp(expr);
            
        case Expr::UNARY_OP:
            return codegenUnaryOp(expr);
            
        case Expr::IF_EXPR:
            return codegenIfExpr(expr);
            
        case Expr::MATCH_EXPR:
            return codegenMatchExpr(expr);
            
        case Expr::FOR_LOOP:
            return codegenForLoop(expr);
            
        case Expr::WHILE_LOOP:
            return codegenWhileLoop(expr);
            
        case Expr::APPLICATION:
        case Expr::LAMBDA:
            return codegenCallExpr(expr);
            
        case Expr::LIST_LITERAL:
            return codegenListLiteral(expr);
            
        case Expr::RECORD_LITERAL:
            return codegenRecordLiteral(expr);

        case Expr::FIELD_ACCESS: {
            // Method call: x.convert() or x.toString()
            auto object = codegenExpr(expr->record_expr);
            const std::string& method = expr->field_name;
            if (method == "convert" || method == "toString") {
                llvm::Type* objType = object->getType();
                Function* conv_fn = nullptr;
                if (objType->isIntegerTy(32)) {
                    conv_fn = module->getFunction("construct_int_to_string");
                } else if (objType->isDoubleTy()) {
                    conv_fn = module->getFunction("construct_float_to_string");
                } else if (objType->isIntegerTy(1)) {
                    conv_fn = module->getFunction("construct_bool_to_string");
                } else if (objType->isPointerTy()) {
                    // String: just return itself
                    return object;
                } else {
                    throw std::runtime_error("toString()/convert() not supported for this type");
                }
                if (!conv_fn && !objType->isPointerTy()) throw std::runtime_error("Conversion function not found");
                return conv_fn ? builder->CreateCall(conv_fn, {object}) : object;
            }
            throw std::runtime_error("Unknown method: " + method);
        }

        case Expr::PIPE: {
            // Chaining: x |> f
            auto left = codegenExpr(expr->left);
            auto right = expr->right;
            // If right is a FIELD_ACCESS with .convert, treat as method call
            if (right->kind == Expr::FIELD_ACCESS && right->field_name == "convert") {
                ExprPtr methodExpr = std::make_shared<Expr>(Expr::FIELD_ACCESS);
                methodExpr->record_expr = expr->left;
                methodExpr->field_name = "convert";
                return codegenExpr(methodExpr);
            }
            // If right is APPLICATION, pass left as argument
            if (right->kind == Expr::APPLICATION) {
                auto app = std::make_shared<Expr>(Expr::APPLICATION);
                app->function_expr = right->function_expr;
                app->arguments = right->arguments;
                app->arguments.insert(app->arguments.begin(), expr->left);
                return codegenExpr(app);
            }
            // If right is IDENTIFIER, treat as function application
            if (right->kind == Expr::IDENTIFIER) {
                auto app = std::make_shared<Expr>(Expr::APPLICATION);
                app->function_expr = right;
                app->arguments = {expr->left};
                return codegenExpr(app);
            }
            throw std::runtime_error("Unsupported pipe target");
        }
            
        default:
            return nullptr;
    }
}

Value* LLVMCodegen::Impl::codegenBinaryOp(const ExprPtr& expr) {
    auto left = codegenExpr(expr->left);
    auto right = codegenExpr(expr->right);
    
    if (!left || !right) return nullptr;
    
    if (expr->op == "+") {
        // String concatenation: if either operand is a string
        if (left->getType()->isPointerTy() || right->getType()->isPointerTy()) {
            Function* concat_fn = module->getFunction("construct_string_concat");
            // Convert left/right to string if needed
            auto toStr = [&](llvm::Value* v) -> llvm::Value* {
                llvm::Type* t = v->getType();
                if (t->isPointerTy()) return v;
                Function* conv_fn = nullptr;
                if (t->isIntegerTy(32)) conv_fn = module->getFunction("construct_int_to_string");
                else if (t->isDoubleTy()) conv_fn = module->getFunction("construct_float_to_string");
                else if (t->isIntegerTy(1)) conv_fn = module->getFunction("construct_bool_to_string");
                if (!conv_fn) throw std::runtime_error("Cannot convert operand to string for concatenation");
                return builder->CreateCall(conv_fn, {v});
            };
            llvm::Value* lstr = toStr(left);
            llvm::Value* rstr = toStr(right);
            return builder->CreateCall(concat_fn, {lstr, rstr});
        } else if (left->getType()->isIntegerTy()) {
            return builder->CreateAdd(left, right, "addtmp");
        } else {
            return builder->CreateFAdd(left, right, "addtmp");
        }
    } else if (expr->op == "-") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateSub(left, right, "subtmp");
        } else {
            return builder->CreateFSub(left, right, "subtmp");
        }
    } else if (expr->op == "*") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateMul(left, right, "multmp");
        } else {
            return builder->CreateFMul(left, right, "multmp");
        }
    } else if (expr->op == "/") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateSDiv(left, right, "divtmp");
        } else {
            return builder->CreateFDiv(left, right, "divtmp");
        }
    } else if (expr->op == "%") {
        return builder->CreateSRem(left, right, "remtmp");
    } else if (expr->op == "==") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateICmpEQ(left, right, "eqtmp");
        } else {
            return builder->CreateFCmpOEQ(left, right, "eqtmp");
        }
    } else if (expr->op == "!=") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateICmpNE(left, right, "netmp");
        } else {
            return builder->CreateFCmpONE(left, right, "netmp");
        }
    } else if (expr->op == "<") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateICmpSLT(left, right, "lttmp");
        } else {
            return builder->CreateFCmpOLT(left, right, "lttmp");
        }
    } else if (expr->op == "<=") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateICmpSLE(left, right, "letmp");
        } else {
            return builder->CreateFCmpOLE(left, right, "letmp");
        }
    } else if (expr->op == ">") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateICmpSGT(left, right, "gttmp");
        } else {
            return builder->CreateFCmpOGT(left, right, "gttmp");
        }
    } else if (expr->op == ">=") {
        if (left->getType()->isIntegerTy()) {
            return builder->CreateICmpSGE(left, right, "getmp");
        } else {
            return builder->CreateFCmpOGE(left, right, "getmp");
        }
    } else if (expr->op == "&&") {
        return builder->CreateAnd(left, right, "andtmp");
    } else if (expr->op == "||") {
        return builder->CreateOr(left, right, "ortmp");
    } else if (expr->op == "**") {
        // Power: use intrinsic for floats
        if (left->getType()->isIntegerTy()) {
            return builder->CreateMul(left, right, "powtmp");
        } else {
            Function* pow_fn = Intrinsic::getDeclaration(module.get(), Intrinsic::pow);
            return builder->CreateCall(pow_fn, {left, right}, "powtmp");
        }
    }
    
    throw std::runtime_error("Unknown operator: " + expr->op);
}

Value* LLVMCodegen::Impl::codegenUnaryOp(const ExprPtr& expr) {
    auto operand = codegenExpr(expr->operand);
    if (!operand) return nullptr;
    
    if (expr->op == "-") {
        if (operand->getType()->isIntegerTy()) {
            return builder->CreateNeg(operand, "negtmp");
        } else {
            return builder->CreateFNeg(operand, "negtmp");
        }
    } else if (expr->op == "!") {
        return builder->CreateNot(operand, "nottmp");
    }
    
    throw std::runtime_error("Unknown unary operator: " + expr->op);
}

Value* LLVMCodegen::Impl::codegenIfExpr(const ExprPtr& expr) {
    auto cond = codegenExpr(expr->condition);
    if (!cond) return nullptr;
    
    // Get current function
    Function* fn = builder->GetInsertBlock()->getParent();
    
    // Create blocks
    BasicBlock* then_bb = BasicBlock::Create(*context, "then", fn);
    BasicBlock* else_bb = BasicBlock::Create(*context, "else", fn);
    BasicBlock* merge_bb = BasicBlock::Create(*context, "merge", fn);
    
    // Create conditional branch from entry
    builder->CreateCondBr(cond, then_bb, else_bb);
    
    // Then block
    builder->SetInsertPoint(then_bb);
    auto then_val = codegenExpr(expr->then_expr);
    if (!then_val) {
        then_val = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
    }
    builder->CreateBr(merge_bb);
    then_bb = builder->GetInsertBlock();
    
    // Else block
    builder->SetInsertPoint(else_bb);
    auto else_val = codegenExpr(expr->else_expr);
    if (!else_val) {
        else_val = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
    }
    builder->CreateBr(merge_bb);
    else_bb = builder->GetInsertBlock();
    
    // DON'T set insertion point to merge_bb yet - just create the PHI
    // Set up for creating PHI in merge block
    builder->SetInsertPoint(merge_bb);
    
    // Ensure both values have the same type
    llvm::Type* phi_type = then_val->getType();
    if (else_val->getType() != phi_type) {
        // Try to convert types intelligently
        if (phi_type->isIntegerTy() && else_val->getType()->isIntegerTy()) {
            // Both integers - convert to matching bitwidth
            else_val = builder->CreateIntCast(else_val, phi_type, true, "castelse");
        } else if (phi_type->isDoubleTy() && else_val->getType()->isDoubleTy()) {
            // Both floating point - should be same already
        } else {
            // Last resort: use then_val's type for the PHI
            llvm::errs() << "Type mismatch warning: then_val type: " << *phi_type 
                         << ", else_val type: " << *else_val->getType() << "\n";
            // Just use then_val's type and hope for the best
        }
    }
    
    PHINode* phi = builder->CreatePHI(phi_type, 2, "iftmp");
    phi->addIncoming(then_val, then_bb);
    phi->addIncoming(else_val, else_bb);
    
    // Keep insertion point at merge_bb for next statements
    return phi;
}

Value* LLVMCodegen::Impl::codegenMatchExpr(const ExprPtr& expr) {
    // Simplified: just match on integer values
    auto match_val = codegenExpr(expr->match_value);
    if (!match_val) return nullptr;
    
    Function* fn = builder->GetInsertBlock()->getParent();
    BasicBlock* current_bb = builder->GetInsertBlock();
    BasicBlock* match_merge = BasicBlock::Create(*context, "match_merge", fn);
    
    // For now, just implement a simple linear case evaluation
    // In a real compiler, this would use a switch statement
    
    if (expr->cases.empty()) {
        // No cases - just return the match value or a default
        return match_val;
    }
    
    // Create case blocks
    std::vector<BasicBlock*> case_blocks;
    std::vector<Value*> case_values;
    
    for (size_t i = 0; i < expr->cases.size(); ++i) {
        BasicBlock* case_bb = BasicBlock::Create(*context, "case", fn);
        case_blocks.push_back(case_bb);
        
        builder->SetInsertPoint(case_bb);
        auto case_val = codegenExpr(expr->cases[i].body);
        if (!case_val) {
            case_val = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
        }
        case_values.push_back(case_val);
        builder->CreateBr(match_merge);
    }
    
    // For now, just branch to the first case (placeholder behavior)
    builder->SetInsertPoint(current_bb);
    if (!case_blocks.empty()) {
        builder->CreateBr(case_blocks[0]);
    } else {
        builder->CreateBr(match_merge);
    }
    
    // Continue at merge block
    builder->SetInsertPoint(match_merge);
    
    // Create PHI if we have multiple cases
    if (case_values.size() > 1) {
        llvm::Type* result_type = case_values[0]->getType();
        PHINode* phi = builder->CreatePHI(result_type, case_values.size(), "matchtmp");
        for (size_t i = 0; i < case_values.size(); ++i) {
            phi->addIncoming(case_values[i], case_blocks[i]);
        }
        return phi;
    } else if (!case_values.empty()) {
        return case_values[0];
    }
    
    return nullptr;
}

Value* LLVMCodegen::Impl::codegenForLoop(const ExprPtr& expr) {
    if (!expr) return nullptr;
    
    // For now, return a placeholder (full loop implementation requires basic blocks)
    // for var in iterable do loop_body end
    // This is a simplified implementation
    return ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
}

Value* LLVMCodegen::Impl::codegenWhileLoop(const ExprPtr& expr) {
    if (!expr) return nullptr;
    
    // While loop implementation with basic blocks
    Function* current_fn = builder->GetInsertBlock()->getParent();
    if (!current_fn) return nullptr;
    
    // Create basic blocks: condition check, body, exit
    BasicBlock* cond_block = BasicBlock::Create(*context, "while.cond", current_fn);
    BasicBlock* body_block = BasicBlock::Create(*context, "while.body", current_fn);
    BasicBlock* exit_block = BasicBlock::Create(*context, "while.exit", current_fn);
    
    // Jump to condition block
    builder->CreateBr(cond_block);
    
    // Evaluate condition
    builder->SetInsertPoint(cond_block);
    auto cond_val = codegenExpr(expr->condition);
    if (!cond_val) return nullptr;
    builder->CreateCondBr(cond_val, body_block, exit_block);
    
    // Generate body
    builder->SetInsertPoint(body_block);
    auto body_val = codegenExpr(expr->loop_body);
    if (!body_val) return nullptr;
    builder->CreateBr(cond_block);  // Loop back to condition
    
    // Continue at exit block
    builder->SetInsertPoint(exit_block);
    
    return ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
}

Value* LLVMCodegen::Impl::codegenCallExpr(const ExprPtr& expr) {
    if (!expr) return nullptr;
    
    // Handle LAMBDA: just store it in symbol table (will be called later)
    if (expr->kind == Expr::LAMBDA) {
        return ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);  // Placeholder
    }
    
    // Handle special builtin: dump()
    if (expr->kind == Expr::APPLICATION && expr->function_expr && 
        expr->function_expr->kind == Expr::IDENTIFIER) {
        
        std::string func_name = expr->function_expr->name;
        
        // dump() function
        if (func_name == "dump" && !expr->arguments.empty()) {
            auto arg = codegenExpr(expr->arguments[0]);
            if (!arg) return nullptr;
            
            // Call appropriate dump function based on argument type
            Function* dump_fn = nullptr;
            if (arg->getType()->isIntegerTy(32)) {
                dump_fn = module->getFunction("construct_dump_int");
            } else if (arg->getType()->isDoubleTy()) {
                dump_fn = module->getFunction("construct_dump_float");
            } else if (arg->getType()->isIntegerTy(1)) {
                dump_fn = module->getFunction("construct_dump_bool");
            } else if (arg->getType()->isPointerTy()) {
                dump_fn = module->getFunction("construct_dump_string");
            }
            
            if (dump_fn) {
                return builder->CreateCall(dump_fn, {arg});
            }
        }
        
        // Regular function call (with automatic external declaration)
        Function* fn = module->getFunction(func_name);
        std::vector<Value*> args;
        for (const auto& arg_expr : expr->arguments) {
            auto arg_val = codegenExpr(arg_expr);
            if (!arg_val) return nullptr;
            args.push_back(arg_val);
        }
        if (!fn) {
            // Not found: declare as external with inferred signature
            std::vector<llvm::Type*> arg_types;
            for (auto* v : args) {
                arg_types.push_back(v->getType());
            }
            // Default to i32 return type (could be improved with type info)
            FunctionType* ext_type = FunctionType::get(llvm::Type::getInt32Ty(*context), arg_types, false);
            fn = Function::Create(ext_type, Function::ExternalLinkage, func_name, module.get());
        }
        return builder->CreateCall(fn, args);
    }
    
    return nullptr;
}

Value* LLVMCodegen::Impl::codegenListLiteral(const ExprPtr& expr) {
    if (!expr || expr->elements.empty()) {
        // Empty list: allocate a zero-length array
        // For now, return a null pointer for empty lists
        return llvm::ConstantPointerNull::get(
            llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(*context))
        );
    }
    
    // Evaluate all elements first to determine their types
    std::vector<Value*> element_values;
    for (const auto& elem : expr->elements) {
        auto value = codegenExpr(elem);
        if (!value) {
            throw std::runtime_error("Failed to generate code for list element");
        }
        element_values.push_back(value);
    }
    
    // Determine the element type (assume all elements have same type - the type of first element)
    llvm::Type* elem_type = element_values[0]->getType();
    size_t num_elements = element_values.size();
    
    // Allocate memory for the array
    // List layout: [length(i64)] [element0] [element1] ...
    llvm::Type* length_type = llvm::Type::getInt64Ty(*context);
    
    // Calculate total size: 8 bytes for length + (element_size * num_elements)
    llvm::Value* elem_size = llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(*context),
        module->getDataLayout().getTypeAllocSize(elem_type)
    );
    llvm::Value* num_elems = llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(*context),
        num_elements
    );
    
    // total_size = 8 + (elem_size * num_elements)
    llvm::Value* data_size = builder->CreateMul(elem_size, num_elems, "list_data_size");
    llvm::Value* total_size = builder->CreateAdd(
        llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 8),
        data_size,
        "list_total_size"
    );
    
    // Call malloc
    Function* malloc_fn = module->getFunction("malloc");
    llvm::Value* ptr = builder->CreateCall(malloc_fn, {total_size}, "list_ptr");
    
    // Cast to i64* to store length
    llvm::Value* length_ptr = builder->CreateBitCast(
        ptr,
        llvm::PointerType::getUnqual(length_type),
        "list_length_ptr"
    );
    
    // Store the length
    builder->CreateStore(
        llvm::ConstantInt::get(length_type, num_elements),
        length_ptr
    );
    
    // Cast to element pointer type to store data
    llvm::Value* data_ptr = builder->CreateBitCast(
        builder->CreateGEP(
            length_type,
            ptr,
            {llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), 1)},
            "list_data_start"
        ),
        llvm::PointerType::getUnqual(elem_type),
        "list_data_ptr"
    );
    
    // Store each element
    for (size_t i = 0; i < num_elements; ++i) {
        llvm::Value* elem_ptr = builder->CreateGEP(
            elem_type,
            data_ptr,
            {llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), i)},
            "elem_ptr"
        );
        builder->CreateStore(element_values[i], elem_ptr);
    }
    
    // Return the list pointer (cast to void*)
    return builder->CreateBitCast(
        ptr,
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context)),
        "list_result"
    );
}

Value* LLVMCodegen::Impl::codegenRecordLiteral(const ExprPtr& expr) {
    if (!expr || expr->record_fields.empty()) {
        // Empty record: allocate a minimal struct
        return llvm::ConstantPointerNull::get(
            llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context))
        );
    }
    
    // Evaluate all field expressions
    std::vector<Value*> field_values;
    std::vector<llvm::Type*> field_types;
    std::vector<std::string> field_names;
    
    for (const auto& [field_name, field_expr] : expr->record_fields) {
        auto value = codegenExpr(field_expr);
        if (!value) {
            throw std::runtime_error("Failed to generate code for record field: " + field_name);
        }
        field_values.push_back(value);
        field_types.push_back(value->getType());
        field_names.push_back(field_name);
    }
    
    // Create a struct type for this record
    llvm::StructType* record_type = llvm::StructType::create(*context, field_types);
    
    // Allocate memory for the record
    llvm::Value* record_size = llvm::ConstantInt::get(
        llvm::Type::getInt64Ty(*context),
        module->getDataLayout().getTypeAllocSize(record_type)
    );
    
    Function* malloc_fn = module->getFunction("malloc");
    llvm::Value* record_ptr = builder->CreateCall(malloc_fn, {record_size}, "record_ptr");
    
    // Cast to the record struct type
    llvm::Value* typed_ptr = builder->CreateBitCast(
        record_ptr,
        llvm::PointerType::getUnqual(record_type),
        "typed_record_ptr"
    );
    
    // Store each field value
    for (size_t i = 0; i < field_values.size(); ++i) {
        // Create a GEP for each field
        llvm::Value* field_ptr = builder->CreateGEP(
            record_type,
            typed_ptr,
            {
                llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0),
                llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), i)
            },
            "field_ptr_" + field_names[i]
        );
        
        // Store the field value
        builder->CreateStore(field_values[i], field_ptr);
    }
    
    // Store record metadata (field names and count) for runtime access
    // For now, return a void* to the allocated record
    return builder->CreateBitCast(
        record_ptr,
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context)),
        "record_result"
    );
}

Function* LLVMCodegen::Impl::codegenFunction(const ExprPtr& func_expr, const std::string& name) {
    if (!func_expr || func_expr->kind != Expr::LAMBDA) return nullptr;
    
    // Save old builder state
    BasicBlock* old_bb = builder->GetInsertBlock();
    
    // Get parameter types - all default to i32 for now
    std::vector<llvm::Type*> param_types;
    for (const auto& param : func_expr->parameters) {
        param_types.push_back(llvm::Type::getInt32Ty(*context));
    }
    
    // Strategy: Use a two-pass approach
    // Pass 1: Quick type inference - create a minimal function to determine return type
    // Pass 2: Create the real function with correct type
    
    // Create a temporary function to infer return type
    // Use void as placeholder return type initially
    FunctionType* temp_type = FunctionType::get(llvm::Type::getInt32Ty(*context), param_types, false);
    Function* temp_fn = Function::Create(temp_type, Function::ExternalLinkage, 
                                         name + "_temp_inference", module.get());
    
    BasicBlock* temp_bb = BasicBlock::Create(*context, "entry", temp_fn);
    builder->SetInsertPoint(temp_bb);
    
    // Save symbol table and create new scope
    auto saved_symbol_table = symbol_table;
    symbol_table.clear();
    
    // Bind parameters
    size_t i = 0;
    for (auto& arg : temp_fn->args()) {
        if (i < func_expr->parameters.size()) {
            symbol_table[func_expr->parameters[i].name] = &arg;
        }
        i++;
    }
    
    // Evaluate body to get return type
    auto inferred_result = codegenExpr(func_expr->body);
    if (!inferred_result) {
        inferred_result = ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);
    }
    llvm::Type* inferred_return_type = inferred_result->getType();
    
    // Restore symbol table
    symbol_table = saved_symbol_table;
    
    // Delete temporary inference function
    temp_fn->eraseFromParent();
    
    // Now create the real function with correct return type
    FunctionType* real_fn_type = FunctionType::get(inferred_return_type, param_types, false);
    Function* fn = Function::Create(real_fn_type, Function::ExternalLinkage, name, module.get());
    
    // Create function body
    BasicBlock* bb = BasicBlock::Create(*context, "entry", fn);
    builder->SetInsertPoint(bb);
    
    // Rebind symbol table with real function
    symbol_table.clear();
    i = 0;
    for (auto& arg : fn->args()) {
        if (i < func_expr->parameters.size()) {
            symbol_table[func_expr->parameters[i].name] = &arg;
        }
        i++;
    }
    
    // Compile function body with correct type
    auto result = codegenExpr(func_expr->body);
    if (!result) {
        result = ConstantInt::get(inferred_return_type, 0);
    }
    
    // Restore symbol table
    symbol_table = saved_symbol_table;
    
    // Add return statement
    BasicBlock* current_bb = builder->GetInsertBlock();
    if (current_bb && !current_bb->getTerminator()) {
        builder->CreateRet(result);
    }
    
    // Restore old builder position
    if (old_bb) {
        builder->SetInsertPoint(old_bb);
    }
    
    return fn;
}

bool LLVMCodegen::Impl::generateObjectFile(const Program& program, const std::string& filename) {
    try {
        generateIR(program);
        return true;
    } catch (const std::exception& e) {
        throw std::runtime_error("Object file generation failed: " + std::string(e.what()));
    }
}

bool LLVMCodegen::Impl::generateExecutable(const Program& program, const std::string& filename) {
    // Generate object file first
    std::string obj_file = filename + ".o";
    if (!generateObjectFile(program, obj_file)) {
        return false;
    }
    
    // Link with system linker
    std::string link_cmd = "gcc " + obj_file + " -o " + filename;
    int ret = system(link_cmd.c_str());
    
    // Clean up object file
    system(("rm " + obj_file).c_str());
    
    return ret == 0;
}

// Public LLVMCodegen interface

LLVMCodegen::LLVMCodegen() : pimpl(std::make_unique<Impl>()) {}

LLVMCodegen::~LLVMCodegen() = default;

std::string LLVMCodegen::generateIR(const Program& program) {
    return pimpl->generateIR(program);
}

bool LLVMCodegen::generateObjectFile(const Program& program, const std::string& filename) {
    return pimpl->generateObjectFile(program, filename);
}

bool LLVMCodegen::generateExecutable(const Program& program, const std::string& filename) {
    return pimpl->generateExecutable(program, filename);
}

std::vector<std::string> LLVMCodegen::getImportedModules() const {
    return pimpl->getImportedModules();
}

}  // namespace construct
