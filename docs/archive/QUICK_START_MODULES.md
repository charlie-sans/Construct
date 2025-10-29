# Quick Start: External Linking & Modules

## Compile with Automatic Stdlib

```bash
# Generate LLVM IR
./build/construct myprogram.ct -o myprogram.ll

# Convert to native (stdlib automatically linked)
llc myprogram.ll -o myprogram.s
gcc myprogram.s build/libconstruct_stdlib.a -o myprogram
./myprogram
```

## Using the Module Build Script

```bash
./compile_with_modules.sh myprogram.ct myprogram
./myprogram
```

## Example: Using stdlib

```construct
import stdlib

let greeting = "Hello, World!"
let count = 42
let pi = 3.14159
let active = true

dump(greeting)
dump(count)
dump(pi)
dump(active)
```

## Key Facts

- **Stdlib is always linked**: No manual steps needed
- **External functions auto-declare**: Call any external function, compiler handles the LLVM `declare`
- **Import is explicit**: Use `import stdlib` to document dependencies
- **No semicolons**: Construct doesn't require statement terminators
- **Supported stdlib functions**: `dump()` for int, float, bool, string

## Files

- `build/libconstruct_stdlib.a` - Compiled stdlib
- `compile_with_modules.sh` - Recommended build script
- `docs/IMPORT_AND_MODULE_SYSTEM.md` - Full documentation
- `examples/test_import.ct` - Working example

## Troubleshooting

**Problem**: "construct_dump_int not found"
**Solution**: Make sure `build/libconstruct_stdlib.a` exists (rebuild with `meson compile -C build`)

**Problem**: "undefined reference to my_function"
**Solution**: Add the library to the gcc link command

**Problem**: IR compiles but executable crashes
**Solution**: Check that the function signature matches (especially return type)
