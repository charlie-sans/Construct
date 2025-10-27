# Construct Import and Module System

## Overview

Construct now features a complete external linking and module import system that allows seamless integration with stdlib functions and custom libraries. When you call a function not defined in your source code, the compiler automatically emits an external function declaration in the LLVM IR, which is then resolved at link time.

## How It Works: Three-Layer System

### 1. External Function Declaration (Compiler Level)

When the compiler encounters a function call like `dump(x)` or `my_lib_function()` that isn't defined in the current module:

- **Before Linking Fix**: The compiler would fail to find the function and crash.
- **After Linking Fix**: The compiler automatically emits an external function declaration (`declare`) in the LLVM IR with an inferred signature based on the call arguments.

**Example LLVM IR Output:**
```llvm
; User-defined function call resolves to external declaration
declare i32 @construct_dump_int(i32)
```

### 2. Standard Library (Build Level)

The Construct stdlib is compiled into a static archive (`build/libconstruct_stdlib.a`) containing:
- `construct_dump_int(i32)` - Dump an integer
- `construct_dump_float(double)` - Dump a floating-point number
- `construct_dump_bool(i1)` - Dump a boolean value
- `construct_dump_string(ptr)` - Dump a string

### 3. Module Import System (Language Level)

Construct supports `import` statements for explicit module specification:

```construct
import stdlib
let x = 42
dump(x)
```

This tells the compiler:
1. This program depends on the `stdlib` module.
2. Link with the corresponding library (`libconstruct_stdlib.a`).
3. Any stdlib functions can be freely called.

## Using the Import System

### Basic Usage

```construct
import stdlib

let greeting = "Hello, World!"
dump(greeting)
```

Compile with:
```bash
./compile_with_modules.sh examples/test_import.ct my_program
```

### Multiple Imports (Future)

```construct
import stdlib
import mylib
import io

let result = mylib_compute(42)
dump(result)
```

### Without Explicit Import

Even without an explicit `import` statement, stdlib is always linked by default, so this also works:

```construct
let x = 100
dump(x)
```

## Build System Integration

### Automatic Linking

- **`compile_to_native.sh`**: Always links with `build/libconstruct_stdlib.a`.
- **`compile_with_modules.sh`**: Enhanced version that handles explicit module imports (extensible).

### Manual Linking (Advanced)

If you want fine-grained control, you can directly use the compiler and link manually:

```bash
# Generate IR
./build/construct myprogram.ct -o myprogram.ll

# Compile to assembly
llc myprogram.ll -o myprogram.s

# Link with specific libraries
gcc myprogram.s build/libconstruct_stdlib.a build/libmylib.a -o myprogram
```

## Adding Your Own Modules

### Step 1: Create Your Library

Write your C++ module in `src/mylib/mylib.cpp`:

```cpp
#include <cstdio>

extern "C" {
    int my_compute(int x) {
        return x * 2;
    }
}
```

### Step 2: Compile to Object/Library

```bash
g++ -c -fPIC -o build/libmylib.o src/mylib/mylib.cpp
ar rcs build/libmylib.a build/libmylib.o
```

### Step 3: Register in Module Manifest

Update `src/srdlib/modules.json`:

```json
{
  "modules": {
    "mylib": {
      "description": "My custom library",
      "library": "libmylib.a",
      "path": "build/libmylib.a",
      "functions": ["my_compute"]
    }
  }
}
```

### Step 4: Update Build Scripts

Edit `compile_with_modules.sh` to add your library to the link command:

```bash
LINK_CMD="gcc $TEMP_S build/libconstruct_stdlib.a build/libmylib.a"
```

### Step 5: Use in Your Code

```construct
import mylib

let result = my_compute(21)
dump(result)
```

Compile and run:

```bash
./compile_with_modules.sh myprogram.ct myprogram
./myprogram
# Output: 42
```

## Module Registry

The `src/srdlib/modules.json` file defines all available modules:

```json
{
  "modules": {
    "stdlib": {
      "description": "Standard library for Construct",
      "library": "libconstruct_stdlib.a",
      "path": "build/libconstruct_stdlib.a",
      "functions": ["construct_dump_int", "construct_dump_float", ...]
    }
  }
}
```

This file can be extended to include:
- Module metadata
- Function signatures
- Dependencies between modules
- Linking options

## Compiler Improvements

### Automatic Declaration Generation

The codegen now includes logic to declare unknown functions on-demand:

```cpp
// In llvm_codegen.cpp: codegenCallExpr()
if (!fn) {
    // Not found: declare as external with inferred signature
    std::vector<llvm::Type*> arg_types;
    for (auto* v : args) {
        arg_types.push_back(v->getType());
    }
    // Default to i32 return type
    FunctionType* ext_type = FunctionType::get(
        llvm::Type::getInt32Ty(*context), 
        arg_types, false);
    fn = Function::Create(ext_type, Function::ExternalLinkage, 
                         func_name, module.get());
}
```

### Module Tracking

The codegen tracks imported modules:

```cpp
std::vector<std::string> imported_modules;  // Track imports
// In codegenStmt() for IMPORT statements:
imported_modules.push_back(stmt->module_name);
```

## Advanced: Dynamic Module Loading

For future versions, consider:

1. **Dynamic Link Libraries (.so)**: Instead of static archives, load shared libraries at runtime.
2. **Module Metadata**: Embed function signatures and dependencies in the library itself.
3. **Import Resolution**: Implement a module search path (e.g., `~/.construct/modules/`, `/usr/local/lib/construct/`).
4. **Module Dependencies**: Handle transitive dependencies (e.g., if `io` depends on `stdlib`, auto-link both).

## Example Workflow

```bash
# 1. Write a program with imports
cat > examples/test_import.ct << 'EOF'
import stdlib
let x = 42
dump(x)
EOF

# 2. Compile with module support
./compile_with_modules.sh examples/test_import.ct test_import

# 3. Run the compiled program
./test_import
# Output: 42

# 4. Check linked libraries
nm test_import | grep construct_dump
# Output shows all stdlib functions are linked
```

## Summary

- **External Linking**: Automatic `declare` generation in LLVM IR enables seamless linking with external libraries.
- **Standard Library**: Pre-compiled `libconstruct_stdlib.a` always linked by default.
- **Module System**: Language-level `import` statements for explicit module specification.
- **Extensible**: Easy to add custom modules by compiling to archives and registering in the module manifest.
- **Future-Ready**: Foundation for dynamic loading, module dependencies, and package management.

For more details on the implementation, see:
- `src/llvm_codegen.cpp` - External function declaration logic
- `src/srdlib/` - Standard library source code and module registry
- `compile_with_modules.sh` - Module-aware build script
