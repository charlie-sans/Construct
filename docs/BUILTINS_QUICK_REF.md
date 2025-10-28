# Quick Reference: Builtin Generation

## TL;DR - What to Do

```bash
# Whenever you modify src/srdlib/stdlib.h, run this:
python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp

# Then rebuild your project:
meson setup builddir
ninja -C builddir
```

## Files at a Glance

| File | Purpose | You Edit? |
|------|---------|-----------|
| `src/srdlib/stdlib.h` | Function declarations | ✏️ YES |
| `generate_builtins.py` | Python generator | ✏️ Rarely |
| `src/srdlib/generated_builtins.cpp` | Generated LLVM code | ❌ NO - auto-generated |
| `src/llvm_codegen.cpp` | Main codegen class | ✏️ Rarely |
| `meson.build` | Build config | ✏️ Once to add .cpp |

## The Loop

```
stdlib.h (define function)
    ↓
    python3 generate_builtins.py
    ↓
generated_builtins.cpp (create LLVM code)
    ↓
    meson setup / ninja build
    ↓
binary (includes your function!)
```

## Adding a New Function

### 1. Edit stdlib.h
```cpp
void construct_my_function(int32_t value);
```

### 2. Run generator
```bash
python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp
```

### 3. Rebuild
```bash
meson setup builddir
ninja -C builddir
```

Done! Your function is now available in the compiled binary.

## Auto-Categorization Rules

Functions are sorted automatically by name:

```
Name Contains → Category Function
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
dump, print, show → registerOutputFunctions()
read, input → registerInputFunctions()
string → registerStringFunctions()
to_ → registerConversionFunctions()
abs, max, min, clamp, round, floor, ceil → registerMathFunctions()
(everything else) → registerUtilityFunctions()
```

### Examples
- `construct_dump_int` → Output
- `construct_read_line` → Input
- `construct_string_length` → String
- `construct_to_string` → Conversion
- `construct_abs` → Math
- `construct_exit` → Utility

## Type Mapping

Automatic C → LLVM type conversion:

```cpp
C Type          →  LLVM Type
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void            →  Type::getVoidTy(*context)
int, int32_t    →  Type::getInt32Ty(*context)
int8_t          →  Type::getInt1Ty(*context)
double          →  Type::getDoubleTy(*context)
char*, const char*  →  PointerType::getUnqual(*context)
```

Need a custom type? Edit the `type_map` in `generate_builtins.py`.

## Generated File Structure

The generated file looks like:

```cpp
#include "llvm_codegen.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/FunctionType.h"
#include <vector>

namespace construct {

// Category 1
void LLVMCodegen::Impl::registerOutputFunctions() {
    // All output functions here
}

// Category 2
void LLVMCodegen::Impl::registerInputFunctions() {
    // All input functions here
}

// ... more categories ...

// Main orchestrator
void LLVMCodegen::Impl::initBuiltins() {
    registerOutputFunctions();
    registerInputFunctions();
    registerStringFunctions();
    registerConversionFunctions();
    registerMathFunctions();
    registerUtilityFunctions();
}

} // namespace construct
```

## Integration into Build

In your `meson.build`, ensure `generated_builtins.cpp` is in the sources:

```meson
construct_sources = [
    'src/llvm_codegen.cpp',
    'src/srdlib/generated_builtins.cpp',  ← Add this if not present
    # ... other .cpp files
]

executable('construct', construct_sources, dependencies: [llvm_dep])
```

## Troubleshooting

### Function not recognized?
- Check it starts with `construct_`
- Check it follows: `ReturnType function_name(args);`
- Check it's in `src/srdlib/stdlib.h`
- Rerun the generator

### Wrong category?
- Categories are assigned by name patterns
- Rename your function to match the right pattern
- Or edit the categorization in `generate_builtins.py`

### Type not mapping?
- Add it to `type_map` dict in `generate_builtins.py`
- Format: `'c_type': 'llvm_type_code'`

### Can't find generated file?
- Make sure you specified output path: 
  ```bash
  python3 generate_builtins.py input.h output.cpp
  ```
- Check it was created in `src/srdlib/generated_builtins.cpp`

### Build fails?
- Ensure `meson.build` includes `generated_builtins.cpp`
- Try: `ninja -C builddir clean` then rebuild
- Check LLVM is properly installed

## Tips & Tricks

### Preview without generating
```bash
python3 generate_builtins.py src/srdlib/stdlib.h | less
```

### Count generated functions
```bash
python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp
# Output: "Generated 59 builtin functions to ..."
```

### Check what functions are in each category
Look at the comments in `generated_builtins.cpp`:
```cpp
// Output Functions
void LLVMCodegen::Impl::registerOutputFunctions() {
    // List of functions in this category
}
```

### Customize categorization
Edit `generate_builtin_code()` in `generate_builtins.py` to change patterns

### Add type mappings
Edit `type_map` in `type_to_llvm()` to handle new C types

## Common Workflow

```bash
# You made changes to stdlib.h
# Now regenerate:
python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp

# See what changed:
git diff src/srdlib/generated_builtins.cpp

# Rebuild and test:
meson setup builddir
ninja -C builddir
./construct --version
```

## Full Documentation

- **BUILTIN_GENERATION.md** - Comprehensive guide
- **BUILTIN_REFACTORING_SUMMARY.md** - What changed
- **BEFORE_AFTER_BUILTINS.md** - Motivation and benefits
