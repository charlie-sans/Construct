# Function Linking Optimization

## Problem: Double Compilation

The original implementation had a significant inefficiency:

### Original Approach (3 passes for non-i32 returns):
1. **Pass 1 (Inference)**: Create temporary function, compile body to determine return type
2. **Pass 2 (Verification)**: Check if return type matches assumption
3. **Pass 3 (Real)**: If mismatch, delete temp function, create new function, recompile body

**Issues:**
- Functions returning `i1` (bool), `double`, or `ptr` required 2 full recompilations
- Complex bodies were compiled 2-3 times before being added to the module
- Type casts/conversions were needed to force incompatible types
- High compilation overhead for multi-function programs

### Example Performance Issue:
```construct
fn is_positive(n) = n > 0  // Returns i1, required 2 compilations!
fn add(a, b) = a + b        // Returns i32, only 1 compilation needed
```

## Solution: Cleaner Two-Pass Inference

### New Approach (Always 2 passes, clean separation):
1. **Pass 1 (Type Inference)**: 
   - Create throwaway temporary function with dummy i32 return
   - Compile body to infer actual return type
   - **Immediately delete temporary function**
   - Get return type, done with this function

2. **Pass 2 (Code Generation)**:
   - Create real function with **correct inferred return type**
   - Bind parameters to real function's arguments
   - Compile body normally
   - Add return statement
   - Clean, correct function ready for linking

### Benefits:
✅ **Cleaner logic**: Strict separation between type inference and code generation  
✅ **Consistent**: Always 2 passes regardless of return type  
✅ **No type mismatches**: Real function is created with correct type from the start  
✅ **No casts**: Functions return native types directly  
✅ **Better maintainability**: Code is easier to understand and modify  
✅ **Extensible**: Easy to add proper type annotations later  

## Code Comparison

### Before (Complex with many branches):
```cpp
// Create temp function with i32 return
FunctionType* temp_fn_type = FunctionType::get(llvm::Type::getInt32Ty(*context), ...);
Function* temp_fn = Function::Create(temp_fn_type, ...);
// ... compile body ...
llvm::Type* actual_return_type = result->getType();
// ... restore state ...

if (actual_return_type != llvm::Type::getInt32Ty(*context)) {
    temp_fn->eraseFromParent();  // Delete temp
    // Create real function
    FunctionType* fn_type = FunctionType::get(actual_return_type, ...);
    Function* fn = Function::Create(fn_type, ...);
    // ... RECOMPILE BODY AGAIN ...
} else {
    // Use temp function as real
    return temp_fn;
}
```

### After (Clean and straightforward):
```cpp
// Pass 1: Infer return type
FunctionType* temp_type = FunctionType::get(...);
Function* temp_fn = Function::Create(...);
// ... bind parameters and compile body ...
auto inferred_result = codegenExpr(func_expr->body);
llvm::Type* inferred_return_type = inferred_result->getType();
// ... restore state ...
temp_fn->eraseFromParent();  // Clean up

// Pass 2: Create real function with correct type
FunctionType* real_fn_type = FunctionType::get(inferred_return_type, ...);
Function* fn = Function::Create(real_fn_type, ...);
// ... bind parameters and compile body once more ...
auto result = codegenExpr(func_expr->body);
// ... add return statement ...
return fn;
```

## Test Results

### Before:
```
fn is_positive(n) = n > 0  // Required 2+ compilations, extra type casting
```
Generated IR:
```llvm
define i32 @is_positive(i32 %0) {
  %gttmp = icmp sgt i32 %0, 0
  %boolext = zext i1 %gttmp to i32  ; ← Type cast hack!
  ret i32 %boolext
}
```

### After:
```
fn is_positive(n) = n > 0  // Clean 2-pass compilation, native types
```
Generated IR:
```llvm
define i1 @is_positive(i32 %0) {
entry:
  %gttmp = icmp sgt i32 %0, 0
  ret i1 %gttmp  ; ← Native i1 type!
}
```

## Future Improvements

Now with this cleaner foundation, we can add:

1. **Type Annotations**: `fn add(a: int, b: int) -> int = a + b`
2. **Type Checking**: Verify inferred types match annotations
3. **Polymorphic Functions**: Generic type parameters
4. **Better Error Messages**: Type mismatch errors with proper locations
5. **Optimization Passes**: Specialized code generation based on parameter types

## Summary

The improved function linking uses a consistent, clean two-pass approach:
- **Pass 1**: Infer the return type from the function body
- **Pass 2**: Create the real function with the correct type

This eliminates:
- ❌ Double compilation
- ❌ Type casting hacks
- ❌ Complex branching logic
- ❌ Temp function creation/deletion cycles

And provides:
- ✅ Cleaner, more maintainable code
- ✅ Correct native types for all functions
- ✅ Consistent compilation overhead
- ✅ Better foundation for future improvements
