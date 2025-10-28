# Builtin Generation System - Complete Documentation

Welcome to the refactored builtin function generation system! This document is an index to all related files and guides.

## 📖 Documentation Files

### Quick Start
- **BUILTINS_QUICK_REF.md** ← **Start here!** - One-page reference with common tasks

### Detailed Guides
- **BUILTIN_GENERATION.md** - Comprehensive technical documentation
- **BUILTIN_REFACTORING_SUMMARY.md** - What changed and why
- **BEFORE_AFTER_BUILTINS.md** - Motivation, benefits, and real-world examples

## 🎯 Quick Navigation

### I want to...

**Add a new function to the standard library**
→ See BUILTINS_QUICK_REF.md - "Adding a New Function"

**Understand how the system works**
→ Read BUILTIN_GENERATION.md - "How It Works"

**See before/after comparison**
→ Check BEFORE_AFTER_BUILTINS.md

**Regenerate builtins after editing stdlib.h**
→ Quick reference at BUILTINS_QUICK_REF.md - "TL;DR"

**Integrate with my build system**
→ See BUILTIN_GENERATION.md - "Integration with Build System"

**Customize type mappings**
→ See BUILTIN_GENERATION.md - "Type Mapping"

**Customize function categories**
→ See BUILTIN_GENERATION.md - "Troubleshooting"

## 📁 Source Files

### Generation
- `generate_builtins.py` - The Python script that does the work

### Definitions
- `src/srdlib/stdlib.h` - Function declarations (single source of truth)

### Generated Code
- `src/srdlib/generated_builtins.cpp` - **AUTO-GENERATED**, do not edit!
  - Contains 6 category registration functions
  - Contains `initBuiltins()` orchestrator
  - Re-run generator whenever stdlib.h changes

### Integration
- `src/llvm_codegen.cpp` - Declares the category functions
- `src/llvm_bindings.cpp` - LLVM bindings skeleton
- `meson.build` - Build configuration (should include generated_builtins.cpp)

## 🚀 The Workflow

```
You modify src/srdlib/stdlib.h
              ↓
Run: python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp
              ↓
Script generates src/srdlib/generated_builtins.cpp
              ↓
Rebuild: meson setup builddir && ninja -C builddir
              ↓
New functions available in compiled binary!
```

## 📊 Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│  Your Code: src/srdlib/stdlib.h                             │
│  (Function declarations - the single source of truth)       │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ↓ (parsed by)
┌─────────────────────────────────────────────────────────────┐
│  Generator: generate_builtins.py                            │
│  (Python script that understands C and LLVM)                │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ↓ (produces)
┌─────────────────────────────────────────────────────────────┐
│  Generated: src/srdlib/generated_builtins.cpp               │
│  - 6 category functions (registerXxx())                     │
│  - initBuiltins() orchestrator                              │
│  - DO NOT EDIT - regenerate instead!                        │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ↓ (linked into)
┌─────────────────────────────────────────────────────────────┐
│  Integration: src/llvm_codegen.cpp                          │
│  (Declares category functions that generated file defines)  │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ↓ (built into)
┌─────────────────────────────────────────────────────────────┐
│  Your Binary: ./construct                                   │
│  (Compiled program with all builtins linked)                │
└─────────────────────────────────────────────────────────────┘
```

## 🔄 Function Categories

Functions are automatically organized into categories:

| # | Category | Pattern | Function Name | Line Count |
|---|----------|---------|---------------|-----------|
| 1 | Output | `dump_*`, `print_*`, `show_*` | `registerOutputFunctions()` | ~155 |
| 2 | Input | `read_*`, `input_*` | `registerInputFunctions()` | ~55 |
| 3 | String | `string_*` | `registerStringFunctions()` | ~163 |
| 4 | Conversion | `to_*` | `registerConversionFunctions()` | ~60 |
| 5 | Math | `abs`, `max`, `min`, `clamp`, `round`, `floor`, `ceil` | `registerMathFunctions()` | ~92 |
| 6 | Utility | Everything else | `registerUtilityFunctions()` | ~~70 |

**Total:** 59 builtin functions organized and registered automatically

## 💾 Regenerating After Changes

```bash
# After editing src/srdlib/stdlib.h, run:
python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp

# Then rebuild your project:
meson setup builddir
ninja -C builddir
```

That's it! New functions are now available.

## ✨ Key Benefits

- ✅ **Single Source of Truth** - All functions defined once in stdlib.h
- ✅ **Automatic Updates** - Run script after modifying stdlib.h
- ✅ **Organized** - Functions grouped logically by category
- ✅ **Maintainable** - No manual LLVM code to maintain
- ✅ **Scalable** - Handles unlimited functions with same structure
- ✅ **Integrable** - Works seamlessly with Meson build system
- ✅ **Trackable** - Generated code shows exactly what changed

## 🐛 Troubleshooting

### Problem: "Function not recognized"
→ Check function name starts with `construct_`
→ See BUILTINS_QUICK_REF.md - Troubleshooting

### Problem: "Build fails after regenerating"
→ Ensure `generated_builtins.cpp` is in meson.build sources
→ Try clean rebuild: `ninja -C builddir clean && ninja -C builddir`

### Problem: "Type not mapping correctly"
→ Add custom type to `type_map` in `generate_builtins.py`
→ See BUILTIN_GENERATION.md - Troubleshooting

### Problem: "Functions in wrong category"
→ Rename function or edit categorization in `generate_builtins.py`
→ See BUILTINS_QUICK_REF.md - Auto-Categorization Rules

## 📝 Example: Adding a Function

1. Edit `src/srdlib/stdlib.h`:
   ```cpp
   void construct_my_function(int32_t value);
   ```

2. Run generator:
   ```bash
   python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp
   ```

3. Rebuild:
   ```bash
   meson setup builddir
   ninja -C builddir
   ```

✅ Your function is now in the binary!

## 🔗 Related Documentation

- LLVM Integration: See `docs/LLVM_INTEGRATION.md`
- Standard Library: See `src/srdlib/README.md`
- Build System: See `meson.build` and `docs/` folder

## 📞 Quick Reference

| Task | Command |
|------|---------|
| Preview generated code | `python3 generate_builtins.py src/srdlib/stdlib.h` |
| Generate to file | `python3 generate_builtins.py src/srdlib/stdlib.h src/srdlib/generated_builtins.cpp` |
| Count functions | Same as above (shows in output message) |
| View generated file | `less src/srdlib/generated_builtins.cpp` |
| Check changes | `git diff src/srdlib/generated_builtins.cpp` |
| Rebuild | `meson setup builddir && ninja -C builddir` |

---

**Last Updated:** After refactoring builtin generation system
**Generated Functions:** 59
**Categories:** 6
**Status:** ✅ Ready to use
