# Quick Start: Auto-completion for Construct Language

## What's New?

Your Construct compiler and VS Code extension now have **intelligent auto-completion**! 

The compiler can list all standard library functions, and the extension uses this to provide:
- ✨ Function auto-completion with parameter hints
- 🔑 Language keyword suggestions
- 📚 Inline documentation
- 🚀 Snippet support for faster coding

## 5-Minute Setup

### 1. Build the Project
```bash
cd /path/to/Construct
meson setup build
cd build
ninja
```

### 2. Verify the Feature Works
```bash
./construct --list-stdlib | head -5
```

You should see JSON output with stdlib functions.

### 3. Test in VS Code

**Option A: Development Mode**
```bash
cd VSCode-Extension/
code . 
# Press F5 to launch extension in debug mode
# Create a test.cst file
```

**Option B: Install Extension**
```bash
cd VSCode-Extension/
npm install  # if needed
code --install-extension .
```

### 4. Try Auto-completion

Create a file `test.cst` and type:

```construct
let print_num = construct_
```

As you type, you should see auto-completion suggestions!

## Using Auto-completion

### Trigger Auto-completion
- Type any stdlib function name starting with `construct_`
- Type language keywords: `let`, `fn`, `if`, `match`, etc.
- Press Ctrl+Space to manually trigger

### Select a Completion
- Click on a suggestion, or
- Use arrow keys and press Enter/Tab

### Use the Snippet
When you select `construct_dump_int`, it expands to:
```
construct_dump_int(${1:int32_t})
```

The `${1:int32_t}` part is editable - just start typing to replace it!

## Example

```construct
// Type: dump
// Select: dump(value)
// Result: dump(${1:value})
// Edit to: dump(42)

fn main =>
  dump(42)
  dump(3.14)
  dump("Hello, World!")
  nl()
```

## Available Functions

The extension knows about these stdlib functions:

**Output Functions**
- `dump(value)` - Output any value (int, float, bool, or string)
- `print(fmt, ...)` - Formatted string printing
- `nl()` - Print a newline

**String Functions**
- `len(s)` - Get string length
- `concat(a, b)` - Concatenate two strings
- `eq(a, b)` - Compare strings for equality
- `substr(s, start, length)` - Extract a substring
- `indexOf(s, char)` - Find character index in string

**Type Conversion**
- `toStr(value)` - Convert to string
- `toInt(s)` - Convert to integer
- `toFloat(s)` - Convert to float
- `toBool(s)` - Convert to boolean

**Math Functions**
- `abs(x)` - Absolute value
- `sqrt(x)` - Square root
- `pow(base, exp)` - Power function
- `max(a, b)` - Maximum of two values
- `min(a, b)` - Minimum of two values

## Language Keywords

Also get auto-completion for:
- `let` - Variable binding
- `fn` - Function definition
- `if` - Conditional
- `else` - Else branch
- `match` - Pattern matching
- `type` - Type definition
- `record` - Record type
- `true`, `false` - Boolean literals

## Refresh Stdlib

If you rebuild the compiler or move it, run:
```
Ctrl+Shift+P → "Construct: Refresh Stdlib"
```

## Troubleshooting

### "No auto-completion showing"

**Check 1: Compiler Found?**
```bash
which construct
# or
ls -la /path/to/build/construct
```

**Check 2: --list-stdlib Works?**
```bash
construct --list-stdlib
```

**Check 3: Extension Output**
VS Code → View → Output → Select "Construct Language Extension"

### "Wrong compiler path"

The extension searches in this order:
1. `./construct` (workspace root)
2. `./build/construct`
3. `./builddir/construct`
4. System paths
5. PATH

If your compiler is elsewhere, create a symlink:
```bash
ln -s /actual/path/to/construct ./construct
```

## Files Changed

**Compiler:**
- ✏️ `src/compiler.h` - New stdlib functions struct
- ✏️ `src/compiler.cpp` - Implementation of stdlib listing
- ✏️ `src/main.cpp` - Added --list-stdlib flag

**Extension:**
- ✏️ `VSCode-Extension/package.json` - Extension configuration
- ✨ `VSCode-Extension/extension.js` - Completion provider (NEW)

## Next Steps

- Explore more Construct language features
- Read `AUTOCOMPLETE_FEATURES.md` for detailed documentation
- Check `docs/AUTOCOMPLETE_IMPLEMENTATION.md` for technical details

## Tips & Tricks

### Speed Up Your Coding

Use snippets to write code faster:
```construct
// Type: construct_d and press Tab
// Gets: construct_dump_int(${1:int32_t})
```

### Discover Available Functions

Type `construct_` to see all output functions, or:
```bash
construct --list-stdlib | jq '.stdlib_functions[] | .name'
```

### Check Function Documentation

Hover over any stdlib function in your code to see its documentation.

---

**Happy coding! 🎉**

For more help, see:
- `VSCode-Extension/AUTOCOMPLETE_FEATURES.md` - Full feature guide
- `docs/AUTOCOMPLETE_IMPLEMENTATION.md` - Technical implementation details
