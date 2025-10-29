# `extern type` Feature - Quick Analysis

## TL;DR - Is It Possible?

**YES! ✅ Very feasible.**

The infrastructure is already 95% there:
- ✅ Lexer recognizes `extern` and `type` keywords
- ✅ Parser pattern established with `extern fn`
- ✅ Field access (`.x`, `.y`) already works for records
- ✅ Type system has RECORD type with fields

**Estimate: 3-4 hours to implement fully**

## What Already Works

```construct
// This ALREADY works:
type Point = {x: Float, y: Float}
let p = {x: 1.0, y: 2.0}
dump(p.x)  // Field access works!
dump(p.y)
```

## What We'd Add

```construct
// NEW - This would become possible:
extern type Point {
    x: Float
    y: Float
}

extern fn create_point(x: Float, y: Float): Point
extern fn distance(p1: Point, p2: Point): Float

let p = create_point(1.0, 2.0)
dump(p.x)  // Dot accessor works
dump(distance(p, p))
```

## Implementation Steps

1. **Parser** - Handle `extern type Name { fields }` syntax
2. **AST** - Add EXTERN_TYPE_DEF statement kind
3. **Codegen** - Register external types in symbol table
4. **Test** - Write examples and verify

## Key Differences: `type` vs `extern type`

| Aspect | `type` | `extern type` |
|--------|--------|---------------|
| Syntax | `type X = {a: T, b: T}` | `extern type X { a: T, b: T }` |
| Layout | Construct-defined | C-defined (must match C struct) |
| Alignment | Rust/Construct rules | C struct rules |
| Field Mutation | Can define | Must match C behavior |
| C Interop | No | Yes! |

## Real-World Example

```construct
// C side: struct Point { float x; float y; };

// Construct side:
extern type Point {
    x: Float
    y: Float
}

extern fn new_point(x: Float, y: Float): Point
extern fn print_point(p: Point): Void

let p = new_point(3.14, 2.71)
print_point(p)  // Passes to C function
dump(p.x)       // Read field
```

## What We DON'T Need to Build (Already Exists)

- ✅ Lexer tokens
- ✅ Field access syntax
- ✅ Type inference for records
- ✅ LLVM codegen for field access
- ✅ Record type representation

## What Needs New Code

- 🔨 Parser modification (~50 lines)
- 🔨 AST update (~3 lines)
- 🔨 Codegen handler (~20 lines)
- 🔨 Type checking (~10 lines)

**Total: ~80 lines of new/modified code**

---

## Implementation Approach Options

### Option A: Minimal MVP (Recommended)
- Just parsing and type registration
- Field access already works
- ~2 hours
- Can add features later

### Option B: Full Featured
- Field mutation support
- Constructor helpers
- Method syntax
- ~4 hours

### Option C: Just for Fun
- Do both now!
- You get maximum flexibility
- ~5-6 hours

## Which Approach Do You Prefer?

1. **Start with MVP** - Get it working, iterate later
2. **Full featured** - Do it all at once
3. **Something in between** - Specify what you want

Let me know and I'll implement it! 🚀
