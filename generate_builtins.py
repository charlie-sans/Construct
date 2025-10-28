#!/usr/bin/env python3
"""
Generate LLVM function declarations from stdlib.h
This script parses stdlib.h and generates category-based builtin registration functions.

Each function category (output, input, string, etc.) gets its own registration function:
- registerOutputFunctions()
- registerInputFunctions()
- registerStringFunctions()
- registerConversionFunctions()
- registerMathFunctions()
- registerUtilityFunctions()

The initBuiltins() function calls all category functions in sequence.

Usage:
    # Print to stdout
    python3 generate_builtins.py src/srdlib/stdlib.h
    
    # Generate to file
    python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp
"""

import re
import sys

def parse_stdlib_header(filename):
    """Parse stdlib.h and extract function declarations."""
    functions = []
    
    with open(filename, 'r') as f:
        content = f.read()
    
    # Pattern to match function declarations
    # Examples:
    #   void construct_dump_int(int32_t value);
    #   const char* construct_read_line(void);
    #   int construct_print(const char* fmt, ...);
    pattern = r'(const\s+)?(\w+(?:\s*\*)?)\s+(\w+)\s*\((.*?)\);'
    
    for match in re.finditer(pattern, content):
        const_prefix = match.group(1) or ""
        return_type = (const_prefix + match.group(2)).strip()
        func_name = match.group(3)
        args_str = match.group(4).strip()
        
        # Skip non-construct functions
        if not func_name.startswith('construct_'):
            continue
        
        # Parse arguments
        if args_str == 'void' or args_str == '':
            args = []
        else:
            # Simple arg parsing (doesn't handle complex types perfectly)
            args = [arg.strip() for arg in args_str.split(',')]
        
        functions.append({
            'name': func_name,
            'return_type': return_type,
            'args': args
        })
    
    return functions

def type_to_llvm(c_type):
    """Convert C type to LLVM type generation code."""
    type_map = {
        'void': 'llvm::Type::getVoidTy(*context)',
        'int32_t': 'llvm::Type::getInt32Ty(*context)',
        'int8_t': 'llvm::Type::getInt1Ty(*context)',
        'double': 'llvm::Type::getDoubleTy(*context)',
        'int': 'llvm::Type::getInt32Ty(*context)',
        'char*': 'llvm::PointerType::getUnqual(*context)',
        'const char*': 'llvm::PointerType::getUnqual(*context)',
    }
    
    c_type = c_type.strip()
    for key, value in type_map.items():
        if c_type == key or c_type.endswith(key):
            return value
    
    # Default fallback
    if '*' in c_type:
        return 'llvm::PointerType::getUnqual(*context)'
    return 'llvm::Type::getInt32Ty(*context)'

def generate_builtin_code(functions):
    """Generate separate function registration functions for each category."""
    # Group functions by category
    output_funcs = [f for f in functions if 'dump' in f['name'] or 'print' in f['name'] or 'show' in f['name']]
    input_funcs = [f for f in functions if 'read' in f['name'] or 'input' in f['name']]
    string_funcs = [f for f in functions if 'string' in f['name']]
    conversion_funcs = [f for f in functions if 'to_' in f['name']]
    math_funcs = [f for f in functions if any(x in f['name'] for x in ['abs', 'max', 'min', 'clamp', 'round', 'floor', 'ceil'])]
    utility_funcs = [f for f in functions if f not in output_funcs + input_funcs + string_funcs + conversion_funcs + math_funcs]
    
    categories = [
        ("Output Functions", output_funcs, "registerOutputFunctions"),
        ("Input Functions", input_funcs, "registerInputFunctions"),
        ("String Functions", string_funcs, "registerStringFunctions"),
        ("Type Conversion Functions", conversion_funcs, "registerConversionFunctions"),
        ("Math Functions", math_funcs, "registerMathFunctions"),
        ("Utility Functions", utility_funcs, "registerUtilityFunctions"),
    ]
    
    code = """// AUTO-GENERATED: Do not edit manually
// Generated from stdlib.h
// This file contains helper functions to register builtin functions by category

#include "llvm_codegen.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/FunctionType.h"
#include <vector>

namespace construct {

using namespace llvm;

"""
    
    # Generate category registration functions
    for category_name, category_funcs, func_name in categories:
        if category_funcs:
            code += generate_category_function(category_name, category_funcs, func_name)
            code += "\n\n"
    
    # Generate the main initBuiltins function that calls all category functions
    code += generate_init_builtins_function(categories)
    
    code += "\n} // namespace construct\n"
    return code


def generate_category_function(category_name, funcs, func_name):
    """Generate a function that registers all functions in a category."""
    code = f"""// {category_name}
void LLVMCodegen::Impl::{func_name}() {{"""
    
    for func in funcs:
        func_code = generate_function_declaration(func)
        # Remove the extra indentation that generate_function_declaration adds
        lines = func_code.split('\n')
        cleaned_lines = [line[4:] if line.startswith('    ') else line for line in lines]
        code += "\n    " + "\n    ".join(cleaned_lines)
    
    code += "\n}"
    return code


def generate_init_builtins_function(categories):
    """Generate the main initBuiltins function that calls all category functions."""
    code = """// Main initialization function that calls all category registration functions
void LLVMCodegen::Impl::initBuiltins() {
    // Register all function categories"""
    
    for _, funcs, func_name in categories:
        if funcs:
            code += f"\n    {func_name}();"
    
    code += "\n}"
    return code

def generate_function_declaration(func):
    """Generate LLVM function declaration code for a single function."""
    func_name = func['name']
    return_type = func['return_type']
    args = func['args']
    
    # Generate argument types
    arg_types = []
    for arg in args:
        arg_type = arg.split()[-1].split('[')[0]  # Get type, handle arrays
        arg_types.append(type_to_llvm(arg_type))
    
    # Generate function type
    if arg_types:
        args_str = ", ".join(arg_types)
        type_str = f"std::vector<llvm::Type*> {func_name}_args = {{{args_str}}};\n"
        type_str += f"    FunctionType* {func_name}_type = FunctionType::get("
    else:
        type_str = f"    FunctionType* {func_name}_type = FunctionType::get("
    
    return_llvm = type_to_llvm(return_type)
    type_str += f"\n        {return_llvm},\n"
    
    if arg_types:
        type_str += f"        {func_name}_args,\n"
    else:
        type_str += f"        {{}},\n"
    
    type_str += f"        false\n"
    type_str += f"    );\n"
    type_str += f'    Function::Create({func_name}_type, Function::ExternalLinkage, "{func_name}", module.get());\n'
    
    return "    " + type_str.replace("\n", "\n    ")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 generate_builtins.py <stdlib.h> [output_file]")
        print()
        print("If output_file is specified, writes to that file. Otherwise prints to stdout.")
        sys.exit(1)
    
    stdlib_path = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else None
    
    functions = parse_stdlib_header(stdlib_path)
    code = generate_builtin_code(functions)
    
    if output_file:
        with open(output_file, 'w') as f:
            f.write(code)
        print(f"Generated {len(functions)} builtin functions to {output_file}")
    else:
        print(code)
