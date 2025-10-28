# External Linking and Standard Library Usage in Construct

## Overview
Construct now supports automatic linking with external functions, including a default standard library (stdlib). This enables you to call stdlib functions (like `dump`, `print`, etc.) and link with additional C/C++ libraries, without manual intervention.

## How It Works
- When your Construct code calls a function that is not defined in your source, the compiler emits an external declaration (`declare`) in the LLVM IR.
- During native compilation, the build system automatically links your program with the default stdlib archive (`build/libconstruct_stdlib.a`).
- This means stdlib functions are always available, and you do not need to specify them manually.

## Adding Your Own Modules
- To add your own C/C++ modules, compile them to object files or static libraries and link them in a similar way (edit the build script if needed).
- For future extensibility, an `import` system is planned to allow language-level module imports.

## Example: Using stdlib Functions
```construct
let x = 42;
dump(x); // Calls stdlib dump function
```
No special flags or manual linking are required.

## Advanced: Linking Additional Libraries
- Place your custom library in the `build/` directory (e.g., `build/libmylib.a`).
- Edit `compile_to_native.sh` to add your library to the gcc link command:
  ```sh
  gcc "$TEMP_S" build/libconstruct_stdlib.a build/libmylib.a -o "$OUTPUT_NAME"
  ```

## Planned: Import/Module System
- In the future, you will be able to write `import mylib` in Construct, and the compiler will handle linking automatically.

## Summary
- Stdlib is always linked automatically.
- External function calls are resolved at link time.
- You can extend the stdlib or add your own modules by linking additional libraries.

For more details, see the stdlib source in `src/srdlib/` and the build scripts.
