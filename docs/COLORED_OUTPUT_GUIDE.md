# 🎨 Colored Compiler Output

The Construct compiler now features beautiful, colored output similar to Rust's `rustc`!

## Quick Start

Just compile normally - colors are automatic:

```bash
$ construct main.ct -x
→ Reading: main.ct
✓ Reading source file
→ Tokenizing: source code
  • tokens: 42
✓ Tokenizing (42 tokens)
...
✓ Compilation successful!
```

## Features

✨ **Visual Indicators**
- Green checkmarks for success
- Cyan arrows for step labels  
- Yellow warnings
- Red errors
- Statistics with bullet points
- Section headers with decorative lines

🚀 **Smart Color Detection**
- Auto-detects terminal capabilities
- Disables colors when piping to files
- Respects `NO_COLOR` environment variable
- Works on Linux, macOS, and Windows

## Disabling Colors

Set the `NO_COLOR` environment variable:

```bash
NO_COLOR=1 construct main.ct
```

Or pipe to a file (colors auto-disable):

```bash
construct main.ct > build.log
```

## See It In Action

### Normal Compilation
```
$ construct hello.ct
→ Reading: hello.ct
✓ Reading source file
→ Tokenizing: source code
  • tokens: 23
✓ Tokenizing (23 tokens)
→ Parsing: AST generation
  • statements: 5
✓ Parsing (5 statements)
→ Processing: include directives
✓ Processing includes
→ Compiling: to LLVM IR
✓ LLVM IR generation
→ Writing: LLVM IR to disk
✓ IR file generation (hello.ll)

✓ Compilation successful!
```

### To Executable
```
$ construct hello.ct -x
→ Reading: hello.ct
✓ Reading source file
...
→ Linking: to executable
✓ Executable generation (hello)

✓ Compilation successful!
```

### With Verbose Output
```
$ construct hello.ct -v
→ Reading: hello.ct
✓ Reading source file
...
=== Generated LLVM IR ===
; ModuleID = 'construct'
source_filename = "construct"
...
────────────────────────────────────
```

## Implementation

New files:
- `src/diagnostics.h` - Terminal style and diagnostic classes
- `src/diagnostics.cpp` - Color and formatting implementation

Modified files:
- `src/main.cpp` - Integrated color output throughout
- `meson.build` - Added diagnostics to build

## API

Use in your code:

```cpp
#include "diagnostics.h"

// Print colored messages
Diagnostics::success("Operation completed");
Diagnostics::warning("Check this out");
Diagnostics::error("Something went wrong");

// Print step info
Diagnostics::step("Compiling", "to object file");
Diagnostics::stat("tokens", 42);

// Track progress
ProgressIndicator progress("Building", 5);
progress.completeStep("Lexing", "250 tokens");
progress.completeStep("Parsing", "12 statements");
progress.complete();
```

## Compatibility

Works on any ANSI-compatible terminal:
- Linux terminals
- macOS Terminal
- Windows Terminal
- MSYS2/Git Bash
- VS Code integrated terminal

Colors gracefully disable when unsupported.
