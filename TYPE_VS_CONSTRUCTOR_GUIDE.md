# Type vs Constructor Analysis - raylib.cst Issue

## The Problem

In your `raylib.cst`:

```construct
type Color = {R: int, G: int, B: int, A: int}

let raywhite = Color{R: 245, G: 245, B: 245, A: 255}
//             ^^^^^ ERROR: Unknown identifier: Color
```

**Why it fails:**
- `type Color = ...` defines a TYPE (type annotation only)
- `Color{...}` tries to use `Color` as an IDENTIFIER/CONSTRUCTOR
- These are different things in the current Construct implementation

## Solutions

### Option 1: Use Record Literal Directly (RECOMMENDED for MVP)

```construct
type Color = {R: int, G: int, B: int, A: int}

// Don't use Color as a constructor, use the record literal directly
let raywhite = {R: 245, G: 245, B: 245, A: 255}
```

This works! The record literal creates the value.

### Option 2: Create a Constructor Function

```construct
type Color = {R: int, G: int, B: int, A: int}

// Define a constructor function
let Color = fn(r: int, g: int, b: int, a: int) -> {R: r, G: g, B: b, A: a}

// Now use it
let raywhite = Color(245, 245, 245, 255)
```

But this shadows the type name and isn't ideal.

### Option 3: Use `extern type` for C struct interop (NEWER FEATURE)

```construct
// For C structs, use the NEW extern type feature!
extern type Color {
    R: int
    G: int
    B: int
    A: int
}

// This registers Color as a type that comes from C
// Field access works: color.R, color.G, etc.
```

This is what `extern type` is designed for!

## What's Happening in Construct's Type System

The issue is that Construct distinguishes between:

1. **Type Aliases** (declarations only, not values):
   ```construct
   type Color = {R: int, G: int, B: int, A: int}
   ```

2. **Values** (actual data in memory):
   ```construct
   let raywhite = {R: 245, G: 245, B: 245, A: 255}
   ```

3. **Constructors** (functions that create values):
   ```construct
   let makeColor = fn(r: int, g: int, b: int, a: int) -> {R: r, G: g, B: b, A: a}
   ```

You need to use #2 or #3, not #1 directly.

## Recommended Fix for Your Code

Replace this:

```construct
type Color = {R: int, G: int, B: int, A: int}
// ...
let raywhite = Color{R: 245, G: 245, B: 245, A: 255}
```

With this:

```construct
type Color = {R: int, G: int, B: int, A: int}
// ...
let raywhite = {R: 245, G: 245, B: 245, A: 255}  // Remove Color prefix
```

Or if you want named constructors, define a function:

```construct
type Color = {R: int, G: int, B: int, A: int}

let make_color = fn(r: int, g: int, b: int, a: int) -> {R: r, G: g, B: b, A: a}

// Now use it
let raywhite = make_color(245, 245, 245, 255)
```

## For Raylib Specifically

Your raylib.cst should be:

```construct
// Type definitions
type Vector2 = {X: float, Y: float}
type Vector3 = {X: float, Y: float, Z: float}
type Vector4 = {X: float, Y: float, Z: float, W: float}
type Color = {R: int, G: int, B: int, A: int}
type Image = {data: ptr, width: int, height: int, mipmaps: int, format: int}

// Constants using record literals directly
let raywhite = {R: 245, G: 245, B: 245, A: 255}

// External functions
extern fn InitWindow(width: int, height: int, title: CStr)
extern fn BeginDrawing()
extern fn EndDrawing()
// ... rest of functions
```

## Why This Works Now

When you write `{R: 245, ...}`:
- Parser recognizes it as a RECORD_LITERAL expression
- Codegen creates the record value
- The type annotation is used for type checking

When you write `Color{...}`:
- Parser tries to look up `Color` as an IDENTIFIER
- But `Color` is only a type, not a value
- Error: "Unknown identifier: Color"

## Future Enhancement

In the future, Construct could add:
- Automatic constructor functions from type aliases
- Type constructor syntax like `Color(r, g, b, a)`
- Implicit record construction

But for now, use record literals directly!

---

## Quick Reference: Fixed raylib.cst

```construct
// raylib.cst - Fixed version

// External functions
extern fn InitWindow(width: int, height: int, title: CStr)
extern fn BeginDrawing()
extern fn EndDrawing()
extern fn DrawText(text: CStr, posX: int, posY: int, fontSize: int, color: int)
// ... more functions

// Type definitions (these are type annotations only)
type Vector2 = {X: float, Y: float}
type Vector3 = {X: float, Y: float, Z: float}
type Vector4 = {X: float, Y: float, Z: float, W: float}
type Color = {R: int, G: int, B: int, A: int}
type Image = {data: ptr, width: int, height: int, mipmaps: int, format: int}

// Constants - use record literals directly
let raywhite = {R: 245, G: 245, B: 245, A: 255}
```

This will work! ✅
