# Input Function & Simplified Function Registration

## Part 1: `input()` Function

### Overview

The `input(prompt)` function provides a simple way to read user input with a prompt message.

**Syntax:**
```construct
let name: String = input("Enter your name: ")
showln("Hello, ", name)
```

### Usage

```construct
let name: String = input("What's your name? ")
let age_str: String = input("What's your age? ")
let age: Int = toInt(age_str)

showln("Name: ", name)
showln("Age: ", age)
```

### Technical Details

- **Function Name:** `construct_input(prompt)`
- **Parameter:** `const char* prompt` - The prompt message to display
- **Return Value:** `const char*` - The user input as a string
- **Behavior:** Prints the prompt, reads input from stdin, returns the string

### Compilation

Works seamlessly with the Construct compiler:

```bash
./construct myprogram.ct -c
clang++ -o myprogram myprogram.o ./build/libconstruct_stdlib.so -lm
./myprogram
```

---

## Part 2: Simplified Function Registration

### The Problem

Previously, every standard library function required manual LLVM type declarations in `initBuiltins()`:

```cpp
// Lots of boilerplate for each function...
FunctionType* dump_int_type = FunctionType::get(
    llvm::Type::getVoidTy(*context),
    {llvm::Type::getInt32Ty(*context)},
    false
);
Function::Create(dump_int_type, Function::ExternalLinkage, "construct_dump_int", module.get());
```

### The Solution: Automatic Generation

Use the provided Python script to auto-generate all function registrations from `stdlib.h`:

```bash
python3 generate_builtins.py src/srdlib/stdlib.h
```

### Why This Helps

1. **Single Source of Truth** - Function declarations are in `stdlib.h` only
2. **No Duplication** - No need to maintain both header and LLVM registration separately
3. **Easy to Add Functions** - Add to `stdlib.h`, run the script, done!
4. **Less Boilerplate** - Eliminates hundreds of lines of repetitive code

### How to Add a New Function

1. **Implement it in C** (`src/srdlib/Stream.cpp`):
```cpp
extern "C" const char* construct_my_function(int32_t param) {
    // Implementation...
    return result;
}
```

2. **Declare it in header** (`src/srdlib/stdlib.h`):
```cpp
/// My new function
const char* construct_my_function(int32_t param);
```

3. **Regenerate LLVM registrations** (optional with automation):
```bash
python3 generate_builtins.py src/srdlib/stdlib.h > /tmp/new_builtins.cpp
# Copy relevant sections into llvm_codegen.cpp
```

4. **Add special case handling** (if needed in `llvm_codegen.cpp`):
```cpp
if (func_name == "my_function" && expr->arguments.size() == 1) {
    // Special handling for variadic or type-based routing
}
```

### Future: Full Automation

In the future, we could:

1. **Use Meson build rules** to auto-generate `initBuiltins()` from `stdlib.h`
2. **Generate code at compile-time** so no manual step is needed
3. **Create a header introspection system** to query available functions at runtime

This would make adding new stdlib functions as simple as adding a C function and a header declaration.

### Current Limitations

- **Special case handling** still required for:
  - Variadic functions (show, showln)
  - Type-based routing (dump, abs, max, min, etc.)
  - Complex overloads

- **Code generation** not yet fully automated in the build system
  - Could be integrated into meson.build for automatic generation

### Benefits of This Approach

| Aspect | Before | After |
|--------|--------|-------|
| Function Registration | Manual per function | Automated from header |
| Lines of Boilerplate | ~500 for 50 functions | ~50 with script |
| Adding New Functions | Add impl + header + LLVM type | Add impl + header |
| Maintenance | Update 3 places | Update 2 places |
| Error Prone | High | Low |

---

## Future Integration

### Ideal End State

```meson
# In meson.build
custom_target('builtins_header',
  input: 'src/srdlib/stdlib.h',
  output: 'generated_builtins.cpp',
  command: ['python3', 'generate_builtins.py', '@INPUT@'],
  capture: true
)
```

This would automatically regenerate `initBuiltins()` whenever `stdlib.h` changes.

---

**Status:** ✓ `input()` implemented and tested  
**Status:** ⚠️ Automatic code generation script available (manual usage for now)  
**Date:** October 28, 2025
