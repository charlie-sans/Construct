# Builtin Function Generation System

## Overview

The builtin function generation system allows you to automatically generate LLVM function registrations from your `stdlib.h` header file. This keeps the function declarations and their LLVM bindings synchronized.

## Architecture

### Components

1. **`generate_builtins.py`** - The generator script that parses `stdlib.h` and produces C++ code
2. **`llvm_codegen.cpp`** - Updated to call category-based registration functions
3. **`generated_builtins.cpp`** - Generated output file (created by running the script)

### Category-Based Organization

Functions are automatically organized into categories:

- **Output Functions** - `construct_dump_*`, `construct_print_*`, `construct_show_*`
- **Input Functions** - `construct_read_*`, `construct_input_*`
- **String Functions** - `construct_string_*`
- **Type Conversion Functions** - `construct_to_*`
- **Math Functions** - `construct_abs`, `construct_max`, `construct_min`, etc.
- **Utility Functions** - Everything else

Each category gets its own function:
- `registerOutputFunctions()`
- `registerInputFunctions()`
- `registerStringFunctions()`
- `registerConversionFunctions()`
- `registerMathFunctions()`
- `registerUtilityFunctions()`

The `initBuiltins()` function then simply calls all category functions in sequence.

## Usage

### Generate to stdout

```bash
python3 generate_builtins.py src/srdlib/stdlib.h
```

This prints the generated C++ code to the terminal, useful for previewing.

### Generate to file

```bash
python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp
```

This creates the `generated_builtins.cpp` file containing all category registration functions.

## Integration with Build System

### Option 1: Manual Integration

1. Run the script to generate `src/srdlib/generated_builtins.cpp`
2. Include it in your `meson.build`:

```meson
construct_sources = [
    'src/llvm_codegen.cpp',
    'src/srdlib/generated_builtins.cpp',  # Add this
    # ... other files
]
```

### Option 2: Automatic Generation with Meson

Add a custom target to your `meson.build`:

```meson
# Generate builtins from stdlib.h
builtins_gen = custom_target(
    'generated_builtins',
    input: 'src/srdlib/stdlib.h',
    output: 'generated_builtins.cpp',
    command: [python3, files('generate_builtins.py'), '@INPUT@', '@OUTPUT@'],
    install: false
)

construct_sources = [
    'src/llvm_codegen.cpp',
    builtins_gen,  # Use generated file
    # ... other files
]
```

## How It Works

### Parsing Phase

The `parse_stdlib_header()` function:
- Reads `stdlib.h`
- Extracts all function declarations matching the pattern: `(const\s+)?(\w+(?:\s*\*)?)\s+(\w+)\s*\((.*?)\);`
- Filters to only `construct_*` functions
- Parses return types and arguments

### Generation Phase

The `generate_builtin_code()` function:
1. Categorizes functions by name patterns
2. Generates a separate function for each category
3. Generates the main `initBuiltins()` that calls all category functions

### Type Mapping

C types are automatically mapped to LLVM types:

| C Type | LLVM Type |
|--------|-----------|
| `void` | `Type::getVoidTy(*context)` |
| `int32_t` | `Type::getInt32Ty(*context)` |
| `int8_t` | `Type::getInt1Ty(*context)` |
| `double` | `Type::getDoubleTy(*context)` |
| `int` | `Type::getInt32Ty(*context)` |
| `char*` | `PointerType::getUnqual(*context)` |
| `const char*` | `PointerType::getUnqual(*context)` |
| `*` (pointer) | `PointerType::getUnqual(*context)` |

## Example Output

For a small set of functions, the generated code looks like:

```cpp
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
    // construct_dump_int
    FunctionType* construct_dump_int_type = FunctionType::get(
        Type::getVoidTy(*context),
        {Type::getInt32Ty(*context)},
        false
    );
    Function::Create(construct_dump_int_type, Function::ExternalLinkage, "construct_dump_int", module.get());
    
    // ... more output functions
}

// Input Functions
void LLVMCodegen::Impl::registerInputFunctions() {
    // ... input function registrations
}

// ... more categories

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
```

## Benefits

1. **Single Source of Truth** - Function declarations are in `stdlib.h` only
2. **Maintainability** - Add new functions to `stdlib.h`, regenerate, rebuild
3. **Organization** - Functions grouped logically by category
4. **Automation** - Can be integrated into build pipeline with Meson
5. **Type Safety** - LLVM types properly mapped from C types

## Workflow

1. Add/modify function declarations in `src/srdlib/stdlib.h`
2. Run: `python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp`
3. Rebuild your project
4. Done! The new functions are available to the LLVM code generator

## Troubleshooting

### Script doesn't recognize my function

Make sure it:
- Is in `stdlib.h`
- Starts with `construct_`
- Follows the pattern: `ReturnType function_name(arg_types);`

### Type mapping not working

Add your type to the `type_map` dictionary in `type_to_llvm()`:

```python
type_map = {
    'void': 'llvm::Type::getVoidTy(*context)',
    'int32_t': 'llvm::Type::getInt32Ty(*context)',
    # ... add more types here
}
```

### Function categorization is wrong

Modify the categorization logic in `generate_builtin_code()` to change how functions are grouped.
