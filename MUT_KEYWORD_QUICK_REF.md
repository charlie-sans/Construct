# Mutable Variables - Quick Reference

## Basic Syntax

### Declare Mutable Variable
```construct
let mut x = 10
```

### Assign to Mutable Variable
```construct
x = 20
```

### With Type Annotation
```construct
let mut count: Int = 0
```

## Examples

### Counter
```construct
let mut i = 0
i = i + 1
dump(i)  // 1
```

### Accumulator
```construct
let mut sum = 0
sum = sum + 5
sum = sum + 3
sum = sum + 2
dump(sum)  // 10
```

### Float Mutation
```construct
let mut x: Float = 1.5
x = x * 2.0
dump(x)  // 3.0
```

### String Mutation
```construct
let mut msg: String = "Hello"
msg = msg + " World"
dump(msg)  // "Hello World"
```

## Rules

1. **Declare with `mut`**: Only mutable variables can be reassigned
   ```construct
   let x = 5        // Cannot reassign
   let mut y = 5    // Can reassign
   ```

2. **Assignment Only to Identifiers**: Can't assign to expressions
   ```construct
   x = 10           // OK
   (x + 1) = 10     // ERROR
   ```

3. **Type Must Match**: Variable type is fixed at declaration
   ```construct
   let mut x: Int = 5
   x = 10           // OK: Int
   x = "hello"      // ERROR: String != Int
   ```

4. **No Reassignment Without `mut`**:
   ```construct
   let x = 5
   x = 10           // ERROR: Cannot assign to immutable variable
   ```

## Common Patterns

### Loop Counter
```construct
let mut i = 0
i = i + 1
i = i + 1
i = i + 1
dump(i)
```

### State Accumulation
```construct
let mut state = 0
state = state + 100
state = state - 50
state = state + 25
dump(state)  // 75
```

### Progressive Calculation
```construct
let mut result: Double = 0.0
result = result + 1.5
result = result * 2.0
result = result - 0.5
dump(result)  // 2.5
```

### Conditional Update
```construct
let mut x = 10
let cond = true
if cond then x = 20 else x = 30
dump(x)  // 20
```

## What's NOT Supported (Yet)

❌ Compound assignment operators:
```construct
x += 1          // Not supported yet
x -= 1          // Not supported yet
x *= 2          // Not supported yet
x /= 2          // Not supported yet
```

❌ Pre/post increment:
```construct
x++             // Not supported
++x             // Not supported
```

❌ Mutable references:
```construct
&mut x          // Not supported (Rust-style)
```

❌ Mutable fields:
```construct
obj.field = 10  // Not supported (need struct mutation)
```

## Performance

- **Immutable variables**: No overhead, direct values
- **Mutable variables**: One LLVM `load` instruction per access
- **Optimization**: LLVM can eliminate redundant loads

## Memory Model

```
Immutable Variable (let x = 5):
  [Value directly in register/SSA]

Mutable Variable (let mut x = 5):
  [Stack memory (alloca)]
    |
    v
  [Loaded into register on each use]
```

## Scope Rules

Mutable variables live for the scope they're declared in:
```construct
let mut x = 0
x = 1
dump(x)        // x is accessible here
// x no longer accessible after function/block ends
```

## Type Inference

Type annotation is optional; can be inferred:
```construct
let mut x = 5           // Inferred as Int
let mut y = 3.14        // Inferred as Float
let mut z: Double = 3.14  // Explicit Double
```

## Error Messages

**Assignment to immutable variable**:
```
Error: Cannot assign to immutable variable 'x'. Use 'mut' keyword when declaring: let mut x
```

**Invalid assignment target**:
```
Error: Assignment target must be an identifier (mutable variable)
```

**Unknown variable**:
```
Error: Unknown identifier: x
```

## Comparison: Immutable vs Mutable

| Feature | Immutable | Mutable |
|---------|-----------|---------|
| Declaration | `let x = 5` | `let mut x = 5` |
| Reassignment | ❌ Error | ✅ Yes |
| Performance | Slightly faster | Tiny overhead |
| Safety | Safer (no surprises) | More flexible |
| Common Use | Pure values | State machines |

## Best Practices

1. **Prefer immutable** when possible (safer, clearer intent)
2. **Use `mut` only when needed** (for stateful operations)
3. **Keep mutable scope small** (reduce bugs)
4. **Avoid reassigning types** (stick to one type per variable)
5. **Document why mutable** (add comments if not obvious)

```construct
// Good: Clear intent
let mut counter = 0
counter = counter + 1

// Avoid: Unnecessary mut if not using reassignment
let mut x = 5  // Just use: let x = 5

// Good: Small scope
let mut sum = 0
sum = sum + 10
dump(sum)

// Avoid: Large scope with many mutations
let mut state = initial
// ... lots of operations
state = state + 1
// ... more operations
// (hard to track all changes)
```

## Related Features

- **Immutable bindings**: `let x = value`
- **Type annotations**: `let x: Type = value`
- **Assignments**: `x = new_value` (mutable only)
- **Binary operators**: `x = x + 1`, `x = x - 1`, etc.
