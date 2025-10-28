// AUTO-GENERATED: Do not edit manually
// Generated from stdlib.h
// This file contains helper functions to register builtin functions by category

#include "llvm_codegen.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/FunctionType.h"
#include <vector>

namespace construct {

using namespace llvm;

// Output Functions
void LLVMCodegen::Impl::registerOutputFunctions() {
    std::vector<llvm::Type*> construct_dump_int_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_int_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_int_args,
            false
        );
        Function::Create(construct_dump_int_type, Function::ExternalLinkage, "construct_dump_int", module.get());
    
    std::vector<llvm::Type*> construct_dump_float_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_float_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_float_args,
            false
        );
        Function::Create(construct_dump_float_type, Function::ExternalLinkage, "construct_dump_float", module.get());
    
    std::vector<llvm::Type*> construct_dump_bool_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_bool_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_bool_args,
            false
        );
        Function::Create(construct_dump_bool_type, Function::ExternalLinkage, "construct_dump_bool", module.get());
    
    std::vector<llvm::Type*> construct_dump_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_string_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_string_args,
            false
        );
        Function::Create(construct_dump_string_type, Function::ExternalLinkage, "construct_dump_string", module.get());
    
    std::vector<llvm::Type*> construct_dump_int_line_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_int_line_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_int_line_args,
            false
        );
        Function::Create(construct_dump_int_line_type, Function::ExternalLinkage, "construct_dump_int_line", module.get());
    
    std::vector<llvm::Type*> construct_dump_float_line_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_float_line_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_float_line_args,
            false
        );
        Function::Create(construct_dump_float_line_type, Function::ExternalLinkage, "construct_dump_float_line", module.get());
    
    std::vector<llvm::Type*> construct_dump_bool_line_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_bool_line_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_bool_line_args,
            false
        );
        Function::Create(construct_dump_bool_line_type, Function::ExternalLinkage, "construct_dump_bool_line", module.get());
    
    std::vector<llvm::Type*> construct_dump_string_line_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_string_line_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_string_line_args,
            false
        );
        Function::Create(construct_dump_string_line_type, Function::ExternalLinkage, "construct_dump_string_line", module.get());
    
    std::vector<llvm::Type*> construct_print_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_print_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_print_args,
            false
        );
        Function::Create(construct_print_type, Function::ExternalLinkage, "construct_print", module.get());
    
    std::vector<llvm::Type*> construct_print_error_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_print_error_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_print_error_args,
            false
        );
        Function::Create(construct_print_error_type, Function::ExternalLinkage, "construct_print_error", module.get());
    
    std::vector<llvm::Type*> construct_show_int_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_show_int_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_show_int_args,
            false
        );
        Function::Create(construct_show_int_type, Function::ExternalLinkage, "construct_show_int", module.get());
    
    std::vector<llvm::Type*> construct_show_float_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_show_float_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_show_float_args,
            false
        );
        Function::Create(construct_show_float_type, Function::ExternalLinkage, "construct_show_float", module.get());
    
    std::vector<llvm::Type*> construct_show_bool_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_show_bool_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_show_bool_args,
            false
        );
        Function::Create(construct_show_bool_type, Function::ExternalLinkage, "construct_show_bool", module.get());
    
    std::vector<llvm::Type*> construct_show_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_show_string_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_show_string_args,
            false
        );
        Function::Create(construct_show_string_type, Function::ExternalLinkage, "construct_show_string", module.get());
    
    std::vector<llvm::Type*> construct_showln_int_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_showln_int_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_showln_int_args,
            false
        );
        Function::Create(construct_showln_int_type, Function::ExternalLinkage, "construct_showln_int", module.get());
    
    std::vector<llvm::Type*> construct_showln_float_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_showln_float_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_showln_float_args,
            false
        );
        Function::Create(construct_showln_float_type, Function::ExternalLinkage, "construct_showln_float", module.get());
    
    std::vector<llvm::Type*> construct_showln_bool_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_showln_bool_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_showln_bool_args,
            false
        );
        Function::Create(construct_showln_bool_type, Function::ExternalLinkage, "construct_showln_bool", module.get());
    
    std::vector<llvm::Type*> construct_showln_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_showln_string_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_showln_string_args,
            false
        );
        Function::Create(construct_showln_string_type, Function::ExternalLinkage, "construct_showln_string", module.get());
    
        FunctionType* construct_showln_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            {},
            false
        );
        Function::Create(construct_showln_type, Function::ExternalLinkage, "construct_showln", module.get());
    
}

// Input Functions
void LLVMCodegen::Impl::registerInputFunctions() {
        FunctionType* construct_read_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            {},
            false
        );
        Function::Create(construct_read_int_type, Function::ExternalLinkage, "construct_read_int", module.get());
    
        FunctionType* construct_read_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            {},
            false
        );
        Function::Create(construct_read_float_type, Function::ExternalLinkage, "construct_read_float", module.get());
    
        FunctionType* construct_read_bool_type = FunctionType::get(
            llvm::Type::getInt1Ty(*context),
            {},
            false
        );
        Function::Create(construct_read_bool_type, Function::ExternalLinkage, "construct_read_bool", module.get());
    
        FunctionType* construct_read_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            {},
            false
        );
        Function::Create(construct_read_string_type, Function::ExternalLinkage, "construct_read_string", module.get());
    
        FunctionType* construct_read_line_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            {},
            false
        );
        Function::Create(construct_read_line_type, Function::ExternalLinkage, "construct_read_line", module.get());
    
    std::vector<llvm::Type*> construct_read_line_prompt_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_read_line_prompt_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_read_line_prompt_args,
            false
        );
        Function::Create(construct_read_line_prompt_type, Function::ExternalLinkage, "construct_read_line_prompt", module.get());
    
    std::vector<llvm::Type*> construct_input_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_input_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_input_args,
            false
        );
        Function::Create(construct_input_type, Function::ExternalLinkage, "construct_input", module.get());
    
}

// String Functions
void LLVMCodegen::Impl::registerStringFunctions() {
    std::vector<llvm::Type*> construct_dump_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_string_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_string_args,
            false
        );
        Function::Create(construct_dump_string_type, Function::ExternalLinkage, "construct_dump_string", module.get());
    
    std::vector<llvm::Type*> construct_dump_string_line_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_dump_string_line_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_dump_string_line_args,
            false
        );
        Function::Create(construct_dump_string_line_type, Function::ExternalLinkage, "construct_dump_string_line", module.get());
    
        FunctionType* construct_read_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            {},
            false
        );
        Function::Create(construct_read_string_type, Function::ExternalLinkage, "construct_read_string", module.get());
    
    std::vector<llvm::Type*> construct_int_to_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_int_to_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_int_to_string_args,
            false
        );
        Function::Create(construct_int_to_string_type, Function::ExternalLinkage, "construct_int_to_string", module.get());
    
    std::vector<llvm::Type*> construct_float_to_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_float_to_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_float_to_string_args,
            false
        );
        Function::Create(construct_float_to_string_type, Function::ExternalLinkage, "construct_float_to_string", module.get());
    
    std::vector<llvm::Type*> construct_bool_to_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_bool_to_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_bool_to_string_args,
            false
        );
        Function::Create(construct_bool_to_string_type, Function::ExternalLinkage, "construct_bool_to_string", module.get());
    
    std::vector<llvm::Type*> construct_string_length_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_length_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_string_length_args,
            false
        );
        Function::Create(construct_string_length_type, Function::ExternalLinkage, "construct_string_length", module.get());
    
    std::vector<llvm::Type*> construct_string_concat_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_concat_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_string_concat_args,
            false
        );
        Function::Create(construct_string_concat_type, Function::ExternalLinkage, "construct_string_concat", module.get());
    
    std::vector<llvm::Type*> construct_string_equals_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_equals_type = FunctionType::get(
            llvm::Type::getInt1Ty(*context),
            construct_string_equals_args,
            false
        );
        Function::Create(construct_string_equals_type, Function::ExternalLinkage, "construct_string_equals", module.get());
    
    std::vector<llvm::Type*> construct_string_upper_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_upper_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_string_upper_args,
            false
        );
        Function::Create(construct_string_upper_type, Function::ExternalLinkage, "construct_string_upper", module.get());
    
    std::vector<llvm::Type*> construct_string_lower_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_lower_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_string_lower_args,
            false
        );
        Function::Create(construct_string_lower_type, Function::ExternalLinkage, "construct_string_lower", module.get());
    
    std::vector<llvm::Type*> construct_string_trim_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_trim_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_string_trim_args,
            false
        );
        Function::Create(construct_string_trim_type, Function::ExternalLinkage, "construct_string_trim", module.get());
    
    std::vector<llvm::Type*> construct_string_starts_with_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_starts_with_type = FunctionType::get(
            llvm::Type::getInt1Ty(*context),
            construct_string_starts_with_args,
            false
        );
        Function::Create(construct_string_starts_with_type, Function::ExternalLinkage, "construct_string_starts_with", module.get());
    
    std::vector<llvm::Type*> construct_string_ends_with_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_ends_with_type = FunctionType::get(
            llvm::Type::getInt1Ty(*context),
            construct_string_ends_with_args,
            false
        );
        Function::Create(construct_string_ends_with_type, Function::ExternalLinkage, "construct_string_ends_with", module.get());
    
    std::vector<llvm::Type*> construct_string_to_int_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_string_to_int_args,
            false
        );
        Function::Create(construct_string_to_int_type, Function::ExternalLinkage, "construct_string_to_int", module.get());
    
    std::vector<llvm::Type*> construct_string_to_float_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_string_to_float_args,
            false
        );
        Function::Create(construct_string_to_float_type, Function::ExternalLinkage, "construct_string_to_float", module.get());
    
    std::vector<llvm::Type*> construct_string_to_bool_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_bool_type = FunctionType::get(
            llvm::Type::getInt1Ty(*context),
            construct_string_to_bool_args,
            false
        );
        Function::Create(construct_string_to_bool_type, Function::ExternalLinkage, "construct_string_to_bool", module.get());
    
    std::vector<llvm::Type*> construct_show_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_show_string_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_show_string_args,
            false
        );
        Function::Create(construct_show_string_type, Function::ExternalLinkage, "construct_show_string", module.get());
    
    std::vector<llvm::Type*> construct_showln_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_showln_string_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_showln_string_args,
            false
        );
        Function::Create(construct_showln_string_type, Function::ExternalLinkage, "construct_showln_string", module.get());
    
    std::vector<llvm::Type*> construct_string_to_cstr_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_cstr_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_string_to_cstr_args,
            false
        );
        Function::Create(construct_string_to_cstr_type, Function::ExternalLinkage, "construct_string_to_cstr", module.get());
    
}

// Type Conversion Functions
void LLVMCodegen::Impl::registerConversionFunctions() {
    std::vector<llvm::Type*> construct_int_to_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_int_to_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_int_to_string_args,
            false
        );
        Function::Create(construct_int_to_string_type, Function::ExternalLinkage, "construct_int_to_string", module.get());
    
    std::vector<llvm::Type*> construct_float_to_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_float_to_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_float_to_string_args,
            false
        );
        Function::Create(construct_float_to_string_type, Function::ExternalLinkage, "construct_float_to_string", module.get());
    
    std::vector<llvm::Type*> construct_bool_to_string_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_bool_to_string_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_bool_to_string_args,
            false
        );
        Function::Create(construct_bool_to_string_type, Function::ExternalLinkage, "construct_bool_to_string", module.get());
    
    std::vector<llvm::Type*> construct_string_to_int_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_string_to_int_args,
            false
        );
        Function::Create(construct_string_to_int_type, Function::ExternalLinkage, "construct_string_to_int", module.get());
    
    std::vector<llvm::Type*> construct_string_to_float_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_string_to_float_args,
            false
        );
        Function::Create(construct_string_to_float_type, Function::ExternalLinkage, "construct_string_to_float", module.get());
    
    std::vector<llvm::Type*> construct_string_to_bool_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_bool_type = FunctionType::get(
            llvm::Type::getInt1Ty(*context),
            construct_string_to_bool_args,
            false
        );
        Function::Create(construct_string_to_bool_type, Function::ExternalLinkage, "construct_string_to_bool", module.get());
    
    std::vector<llvm::Type*> construct_string_to_cstr_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_string_to_cstr_type = FunctionType::get(
            llvm::PointerType::getUnqual(*context),
            construct_string_to_cstr_args,
            false
        );
        Function::Create(construct_string_to_cstr_type, Function::ExternalLinkage, "construct_string_to_cstr", module.get());
    
}

// Math Functions
void LLVMCodegen::Impl::registerMathFunctions() {
    std::vector<llvm::Type*> construct_abs_int_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_abs_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_abs_int_args,
            false
        );
        Function::Create(construct_abs_int_type, Function::ExternalLinkage, "construct_abs_int", module.get());
    
    std::vector<llvm::Type*> construct_abs_float_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_abs_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_abs_float_args,
            false
        );
        Function::Create(construct_abs_float_type, Function::ExternalLinkage, "construct_abs_float", module.get());
    
    std::vector<llvm::Type*> construct_max_int_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_max_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_max_int_args,
            false
        );
        Function::Create(construct_max_int_type, Function::ExternalLinkage, "construct_max_int", module.get());
    
    std::vector<llvm::Type*> construct_max_float_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_max_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_max_float_args,
            false
        );
        Function::Create(construct_max_float_type, Function::ExternalLinkage, "construct_max_float", module.get());
    
    std::vector<llvm::Type*> construct_min_int_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_min_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_min_int_args,
            false
        );
        Function::Create(construct_min_int_type, Function::ExternalLinkage, "construct_min_int", module.get());
    
    std::vector<llvm::Type*> construct_min_float_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_min_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_min_float_args,
            false
        );
        Function::Create(construct_min_float_type, Function::ExternalLinkage, "construct_min_float", module.get());
    
    std::vector<llvm::Type*> construct_clamp_int_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_clamp_int_type = FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            construct_clamp_int_args,
            false
        );
        Function::Create(construct_clamp_int_type, Function::ExternalLinkage, "construct_clamp_int", module.get());
    
    std::vector<llvm::Type*> construct_clamp_float_args = {llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context), llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_clamp_float_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_clamp_float_args,
            false
        );
        Function::Create(construct_clamp_float_type, Function::ExternalLinkage, "construct_clamp_float", module.get());
    
    std::vector<llvm::Type*> construct_round_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_round_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_round_args,
            false
        );
        Function::Create(construct_round_type, Function::ExternalLinkage, "construct_round", module.get());
    
    std::vector<llvm::Type*> construct_floor_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_floor_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_floor_args,
            false
        );
        Function::Create(construct_floor_type, Function::ExternalLinkage, "construct_floor", module.get());
    
    std::vector<llvm::Type*> construct_ceil_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_ceil_type = FunctionType::get(
            llvm::Type::getDoubleTy(*context),
            construct_ceil_args,
            false
        );
        Function::Create(construct_ceil_type, Function::ExternalLinkage, "construct_ceil", module.get());
    
}

// Utility Functions
void LLVMCodegen::Impl::registerUtilityFunctions() {
        FunctionType* construct_newline_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            {},
            false
        );
        Function::Create(construct_newline_type, Function::ExternalLinkage, "construct_newline", module.get());
    
        FunctionType* construct_nl_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            {},
            false
        );
        Function::Create(construct_nl_type, Function::ExternalLinkage, "construct_nl", module.get());
    
    std::vector<llvm::Type*> construct_sleep_ms_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_sleep_ms_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_sleep_ms_args,
            false
        );
        Function::Create(construct_sleep_ms_type, Function::ExternalLinkage, "construct_sleep_ms", module.get());
    
    std::vector<llvm::Type*> construct_sleep_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_sleep_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_sleep_args,
            false
        );
        Function::Create(construct_sleep_type, Function::ExternalLinkage, "construct_sleep", module.get());
    
        FunctionType* construct_clear_screen_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            {},
            false
        );
        Function::Create(construct_clear_screen_type, Function::ExternalLinkage, "construct_clear_screen", module.get());
    
    std::vector<llvm::Type*> construct_exit_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_exit_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_exit_args,
            false
        );
        Function::Create(construct_exit_type, Function::ExternalLinkage, "construct_exit", module.get());
    
    std::vector<llvm::Type*> construct_debug_args = {llvm::Type::getInt32Ty(*context)};
        FunctionType* construct_debug_type = FunctionType::get(
            llvm::Type::getVoidTy(*context),
            construct_debug_args,
            false
        );
        Function::Create(construct_debug_type, Function::ExternalLinkage, "construct_debug", module.get());
    
}

// Main initialization function that calls all category registration functions
void LLVMCodegen::Impl::initBuiltins() {
    // Register all function categories
    registerOutputFunctions();
    registerInputFunctions();
    registerStringFunctions();
    registerConversionFunctions();
    registerMathFunctions();
    registerUtilityFunctions();
}
} // namespace construct
