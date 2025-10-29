# `extern type` Feature Design & Implementation Plan

## Overview

You want to extend the `extern` keyword to support types, enabling C struct interop with dot accessor support:

```construct
extern type Point {
    x: Float
    y: Float
}

// Usage
external_point.x  // Dot accessor
external_point.y
```

## Current State Analysis

### What Already Exists ✅

1. **Lexer** (`src/lexer.cpp`)
   - ✅ `extern` keyword recognized (TokenType::KW_EXTERN)
   - ✅ `type` keyword recognized (TokenType::KW_TYPE)

2. **Parser** (`src/parser.cpp`)
   - ✅ `extern fn` parsing implemented (lines 193-202)
   - ✅ `type` alias parsing implemented (lines 203-209)
   - ✅ Pattern: Check for `extern`, then check for keyword

3. **AST** (`src/ast.h`)
   - ✅ Statement kinds include TYPE_DEF and FUNCTION_DEF
   - ✅ RECORD_LITERAL and FIELD_ACCESS expression kinds exist
   - ✅ Type system has RECORD kind with fields

4. **Code Generation** (`src/llvm_codegen.cpp`)
   - ✅ Field access codegen already works for records

## Proposed Implementation

### Step 1: Update Statement Types
Add to `Statement::Kind`:
```cpp
EXTERN_TYPE_DEF,  // New kind for extern type definitions
```

### Step 2: Extend Parser
Modify `parseStatement()` in `src/parser.cpp`:
```cpp
if (check(TokenType::KW_EXTERN)) {
    is_extern = true;
    advance();
}

// After checking for fn, add:
if (check(TokenType::KW_TYPE)) {
    auto stmt = parseTypeDefinition(is_extern);
    return stmt;
}
```

### Step 3: Implement Type Parsing
```cpp
StmtPtr Parser::parseTypeDefinition(bool is_extern) {
    advance();  // skip 'type'
    
    auto stmt = std::make_shared<Statement>(is_extern ? 
        Statement::EXTERN_TYPE_DEF : 
        Statement::TYPE_DEF);
    
    stmt->name = consume(TokenType::IDENTIFIER, "Expected type name").value;
    
    if (is_extern) {
        // extern type Point { x: Float, y: Float }
        consume(TokenType::LBRACE, "Expected '{' for extern type fields");
        while (!check(TokenType::RBRACE)) {
            std::string field_name = consume(TokenType::IDENTIFIER, "Expected field name").value;
            consume(TokenType::COLON, "Expected ':'");
            TypePtr field_type = parseType();
            stmt->struct_fields.push_back({field_name, field_type});
            
            if (!check(TokenType::RBRACE)) {
                consume(TokenType::COMMA, "Expected ',' or '}'");
            }
        }
        consume(TokenType::RBRACE, "Expected '}'");
    } else {
        // Regular type alias: type Point = {x: Float, y: Float}
        consume(TokenType::EQ, "Expected '='");
        stmt->type_value = parseType();
    }
    
    return stmt;
}
```

### Step 4: Add Fields to AST Statement
```cpp
struct Statement {
    // ... existing members ...
    
    // For struct fields (extern type)
    std::vector<std::pair<std::string, TypePtr>> struct_fields;
};
```

### Step 5: Update Code Generation
In `src/llvm_codegen.cpp`, handle EXTERN_TYPE_DEF:
```cpp
void LLVMCodegen::Impl::codegenStmt(const StmtPtr& stmt) {
    // ... existing cases ...
    
    case Statement::EXTERN_TYPE_DEF:
        // Register the struct type in symbol table
        // This tells the codegen that the type is from external C code
        // When field access is used, it's already handled by FIELD_ACCESS
        symbol_table[stmt->name] = nullptr; // Mark as extern type
        break;
}
```

## Example Usage

### Define Extern Type
```construct
extern type Point {
    x: Float
    y: Float
}

extern type Color {
    r: Int
    g: Int
    b: Int
}
```

### Use with External Functions
```construct
extern fn create_point(x: Float, y: Float): Point
extern fn distance(p1: Point, p2: Point): Float

let p1 = create_point(1.0, 2.0)
let p2 = create_point(4.0, 6.0)

dump(p1.x)  // Access field
dump(p1.y)

let dist = distance(p1, p2)
dump(dist)
```

## Benefits

✅ **Type Safety** - Compiler knows struct layout from declarations
✅ **C Interop** - Can use structs from C libraries
✅ **Natural Syntax** - `obj.field` is familiar to users
✅ **No Runtime Overhead** - Everything compiles to LLVM
✅ **Extensible** - Can add methods later if needed

## Implementation Complexity

| Component | Complexity | Effort |
|-----------|-----------|--------|
| Parser | Low | 30 min |
| AST updates | Low | 10 min |
| Codegen | Low-Medium | 1 hour |
| Testing | Medium | 1-2 hours |
| Documentation | Low | 30 min |

**Total Estimate**: 3-4 hours

## Considerations

### What's Already Handled ✅
- Field access syntax (FIELD_ACCESS expression already exists)
- Type system (RECORD type already defined)
- Record literals (can parse them)

### What Needs Implementation
- Parser: `extern type Name { fields }`
- Codegen: Mark types as external in symbol table
- Runtime: Pass structs between extern functions

### What Could Be Added Later (Not MVP)
- Method syntax: `point.distance(other_point)`
- Constructor functions
- Automatic marshaling
- Nested structs
- Array fields

## Quick Start

To implement this feature:

1. **Update AST** - Add EXTERN_TYPE_DEF to Statement::Kind
2. **Update Parser** - Handle `extern type` syntax
3. **Update Codegen** - Register external types
4. **Write Tests** - Create examples using extern types
5. **Documentation** - Add to README and docs

The hardest part will be ensuring LLVM IR correctly represents the struct layout when passed to/from C code. This might require specifying alignment and padding.

## Questions to Consider

1. Should we support field mutation? `p.x = 5.0`?
2. Should structs be copiable or reference types?
3. Do we need constructor syntax like `Point { x: 1.0, y: 2.0 }`?
4. Should field access be checked at compile-time or runtime?

---

**Ready to implement?** Start with Step 1 (AST updates) and work through the steps sequentially.
