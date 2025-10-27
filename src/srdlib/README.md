# Standard Library (stdlib) Organization

## Overview

The Construct standard library is organized in `src/srdlib/` with a modular structure for easy extension.

## Current Structure

```
src/srdlib/
├── stdlib.h              # Public declarations (C-compatible)
└── Stream.cpp            # I/O implementation
```

## Files

### stdlib.h - Public Interface

**Purpose**: Declares all stdlib functions available to compiled Construct programs.

**Content**:
- `construct_dump_int(int32_t)`
- `construct_dump_float(double)`
- `construct_dump_bool(int8_t)`
- `construct_dump_string(const char*)`
- `construct_newline()`
- `construct_print(const char*, ...)`

**Language**: C with C++ extern "C" wrapper
**Location**: `src/srdlib/stdlib.h`

### Stream.cpp - I/O Implementation

**Purpose**: Implements output functions using printf.

**Functions**:
- `construct_dump_int()` - Print integer
- `construct_dump_float()` - Print float (using %g format)
- `construct_dump_bool()` - Print "true" or "false"
- `construct_dump_string()` - Print string
- `construct_newline()` - Print newline
- `construct_print()` - Generic printf wrapper

**Language**: C++
**Location**: `src/srdlib/Stream.cpp`

## How It Works

### Compilation to LLVM

When you write:
```construct
dump(42)
```

The compiler generates:
```llvm
declare void @construct_dump_int(i32)

define i32 @main() {
entry:
  call void @construct_dump_int(i32 42)
  ret i32 0
}
```

### Linking

The stdlib object files are compiled and linked:
```bash
# Compile stdlib
g++ -c src/srdlib/Stream.cpp -o Stream.o

# Link with program
g++ program.o Stream.o -o program
```

## Future Modules

The stdlib can be extended with:

### input.cpp
```cpp
int32_t construct_read_int();
double construct_read_float();
const char* construct_read_string();
```

### math.cpp
```cpp
int32_t construct_abs(int32_t);
double construct_sqrt(double);
int32_t construct_max(int32_t, int32_t);
int32_t construct_min(int32_t, int32_t);
```

### string.cpp
```cpp
const char* construct_concat(const char*, const char*);
int32_t construct_length(const char*);
const char* construct_substring(const char*, int32_t, int32_t);
```

### memory.cpp
```cpp
void* construct_malloc(int32_t);
void construct_free(void*);
```

## Adding New Functions

### Step 1: Declare in stdlib.h
```cpp
int32_t construct_add_one(int32_t value);
```

### Step 2: Implement in new module (e.g., math.cpp)
```cpp
extern "C" int32_t construct_add_one(int32_t value) {
    return value + 1;
}
```

### Step 3: Update meson.build
```meson
sources = [
    'src/srdlib/Stream.cpp',
    'src/srdlib/math.cpp',
]
```

### Step 4: Update compiler to recognize it
In `llvm_codegen.cpp`:
```cpp
if (func_name == "add_one" && expr->arguments.size() == 1) {
    // Handle add_one builtin
}
```

## Builtin Functions vs Regular Functions

### Builtins (like dump)
- Recognized by compiler
- Type-dispatched at compile time
- No function declaration needed
- Example: `dump(42)`

### Regular Functions (planned)
- User-defined or library functions
- Called like normal functions
- Require declarations
- Example: `myFunc(x)`

## Design Principles

1. **Minimal**: Only essential functions in core stdlib
2. **Efficient**: Direct C implementation
3. **Type-Safe**: Each type has its own function
4. **Extensible**: Easy to add modules
5. **C-Compatible**: Can link with C libraries

## Building with Stdlib

```bash
# Configure
meson setup build

# Compile (includes stdlib automatically)
meson compile -C build

# Uses:
./build/construct program.ct
# Generates: program.ll
```

## Integration with Compiler

### llvm_codegen.cpp
```cpp
void LLVMCodegen::Impl::initBuiltins() {
    // Declare stdlib functions to LLVM
    Function::Create(printf_type, Function::ExternalLinkage, "printf", module.get());
    Function::Create(dump_int_type, Function::ExternalLinkage, "construct_dump_int", module.get());
    // ... etc
}
```

### In codegenCallExpr()
```cpp
if (func_name == "dump") {
    // Dispatch based on argument type
    if (arg->getType()->isIntegerTy(32)) {
        fn = module->getFunction("construct_dump_int");
    }
    // ... etc
}
```

## Example: Complete Program

**Program (program.ct):**
```construct
dump(42)
dump("\n")
dump("Hello")
```

**Generated IR (program.ll):**
```llvm
@.str = private unnamed_addr constant [6 x i8] c"Hello\00"

declare void @construct_dump_int(i32)
declare void @construct_dump_string(ptr)

define i32 @main() {
entry:
  call void @construct_dump_int(i32 42)
  call void @construct_dump_string(ptr @.str)
  ret i32 0
}
```

**Compilation:**
```bash
./build/construct program.ct
```

**Output:**
```llvm
# (generates program.ll)
```

## Future: Dynamic Library Loading

The stdlib can be extended to support:
- Dynamic library loading (dlopen)
- FFI (Foreign Function Interface)
- C library bindings
- Reflection and introspection

---

**Last Updated**: October 27, 2025
**Version**: 0.1.0
**Status**: Active Development
