# Construct Include System - User Guide

## Quick Start

### Basic Usage
Include files from your project using the `include` directive:

```construct
include ["lib/math.ct"]

let result = square(5)
dump(result)
```

### Supported Patterns

#### Direct File Inclusion
```construct
include ["lib/math.ct"]
```

#### Multiple Files
```construct
include ["lib/math.ct", "lib/utils.ct", "lib/helpers.ct"]
```

#### Wildcard Patterns
```construct
include ["lib/*.ct"]           // All .ct files in lib/
include ["src/**/*.ct"]        // Recursive: all .ct in src/ and subdirectories
include ["*.cst"]              // All .cst files in current directory
```

#### Mixed Patterns
```construct
include ["lib/core.ct", "utils/*.ct", "**/*.cst"]
```

## How It Works

1. **Parser recognizes the include directive** during compilation
2. **FileIncluder resolves patterns** to concrete file paths
3. **Included files are parsed** into AST nodes
4. **Programs are merged** - included statements are prepended to main program
5. **Compilation continues** with merged AST

## Example: Organizing Code into Modules

### Directory Structure
```
project/
├── main.ct              // Main program
└── lib/
    ├── math.ct          // Math utilities
    └── string.ct        // String utilities
```

### lib/math.ct
```construct
let square = fn(x: Int) -> x * x
let cube = fn(x: Int) -> x * x * x
let factorial = fn(n: Int) -> if n <= 1 then 1 else n * factorial(n - 1)
```

### lib/string.ct
```construct
let to_upper = fn(s: Str) -> s     // TODO: Implement
let reverse = fn(s: Str) -> s      // TODO: Implement
```

### main.ct
```construct
include ["lib/math.ct", "lib/string.ct"]

let x = 5
let y = square(x)
let z = cube(y)

dump(y)
dump(z)
```

## Pattern Matching Details

### Supported Extensions
- `.ct` - Construct source files
- `.cst` - Construct script files
- Any file path (resolved relative to source file location)

### Wildcard Rules
| Pattern | Matches |
|---------|---------|
| `*.ct` | All `.ct` files in current directory and subdirectories |
| `lib/*.ct` | All `.ct` files in `lib/` and its subdirectories |
| `src/**/*.ct` | All `.ct` files in `src/` and any subdirectories |
| `math.ct` | Exact file path `math.ct` |
| `lib/core.ct` | Exact file path `lib/core.ct` |

### Path Resolution
- Paths are relative to the **source file location**
- Absolute paths are supported
- `.` refers to source directory
- `..` refers to parent directory

## Error Handling

### Missing Files
If a file cannot be found, a warning is issued but compilation continues:
```
Warning: Include failed - Include file not found: lib/nonexistent.ct
```

### Parse Errors in Included Files
If an included file has syntax errors, they are reported with the filename.

## Tips & Best Practices

✅ **DO:**
- Organize library functions in separate files
- Use wildcard patterns for plugin directories
- Place includes at the top of your main file
- Use descriptive file names for clarity

❌ **DON'T:**
- Use circular includes (will be processed sequentially)
- Include files with side effects in main expressions
- Forget that included statements execute in order

## Limitations (Current Version)

- No circular include protection yet (avoid circular dependencies)
- No namespace isolation (all symbols in global scope)
- No selective imports (whole file is imported)
- No include guards or pragma once

## Future Features

- Circular include detection with error messages
- Include guards: `#pragma once`
- Selective imports: `include ["lib/math.ct" as math]`
- Namespace support: `include ["lib/math.ct" in math]`
- Conditional inclusion: `include ["optional/feature.ct" if DEBUG]`

## Examples in Repository

Check the `examples/` directory for complete examples:
- `examples/test_include.ct` - Single file inclusion
- `examples/test_wildcard.ct` - Wildcard patterns
- `examples/test_multiple.ct` - Multiple explicit files
- `examples/test_missing.ct` - Error handling

Run the verification script:
```bash
./test_includes.sh
```

## Troubleshooting

**Problem:** "Include file not found"
- **Solution:** Check file path is relative to source directory, not working directory

**Problem:** Functions from included file not found
- **Solution:** Ensure included file has proper Construct syntax and compiles independently

**Problem:** Include directive not recognized
- **Solution:** Use bracket syntax: `include ["file.ct"]` not `include "file.ct"`

**Problem:** Functions defined twice
- **Solution:** Include each file only once; consider using `**/*.ct` for recursive patterns

## See Also
- [Include System Implementation](./INCLUDE_SYSTEM.md)
- [Construct Language Reference](./README_CURRENT.md)
- [Quick Reference Guide](./QUICK_REFERENCE.md)
