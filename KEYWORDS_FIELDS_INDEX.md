# Keywords as Field Names - Documentation Index

## Quick Links

**TL;DR Status**: ✅ Parser ready / ⏳ Codegen in progress

### Key Documents

1. **KEYWORDS_FIELDS_IMPLEMENTATION_REPORT.md** - START HERE
   - What was implemented
   - Code changes explained
   - What works and doesn't
   - Workarounds

2. **KEYWORDS_AS_FIELDS_STATUS.md**
   - Detailed status report
   - Architecture notes
   - Next steps for completion
   - Testing information

3. **DOT_ACCESSOR_KEYWORDS.md**
   - User guide for using keywords as field names
   - Pattern recommendations
   - SDL/Raylib binding examples
   - Best practices

4. **KEYWORDS_AS_FIELDS_COMPLETE.md**
   - Original implementation summary
   - File modifications list
   - Implementation details

## What Happened

### The Request
User asked: "How can we handle dot accessor methods for things? Specifically accessing `event.type` where `type` is both a reserved keyword and a field name."

### The Solution
Modified the Construct parser to accept reserved keywords as field names in:
1. Type definitions: `type Event = {type: Int}`
2. Field access: `event.type`

### The Status
- ✅ **Parser**: Keywords as field names now parse successfully
- ⏳ **Codegen**: Field extraction needs to be implemented
- ⏳ **Testing**: Parser tests pass, codegen tests need enhancement

## Supported Keywords

Can now use these as field names:
- `type`
- `let`
- `fn`
- `if`
- `match`
- `for`
- `while`

## Code Changes Summary

### File: src/parser.cpp

**Change 1: parseFunctionType()** (~20 lines)
- Added keyword checking in record type field parsing
- Now accepts KW_TYPE, KW_LET, KW_FN, KW_IF, KW_MATCH

**Change 2: parsePostfix()** (~25 lines)
- Added keyword checking in field access expression parsing
- Now accepts KW_TYPE, KW_LET, KW_FN, KW_IF, KW_MATCH, KW_FOR, KW_WHILE

## Example Usage

### Define Type with Keyword Field
```construct
type Event = {
    type: Int,
    let: String,
    fn: Int
}
```

### Create Record Instance
```construct
let event = {
    type: 42,
    let: "test",
    fn: 1
}
```

### Access Fields (Currently Needs Workaround)
```construct
// These parse OK but codegen fails:
dump(event.type)   // ❌ "Unknown method: type"
dump(event.let)    // ❌ "Unknown method: let"

// Workaround - use pattern matching:
let {type: t, let: l} = event
dump(t)            // ✅ Works
dump(l)            // ✅ Works
```

## Next Phase

To complete the feature:

1. **Enhance FIELD_ACCESS codegen** in `src/llvm_codegen.cpp`
   - Track record types through codegen
   - Implement field extraction logic
   - Use LLVM GEP or similar for field access

2. **Add type information propagation**
   - Symbol table tracking variable types
   - Type inference during code generation

3. **Test and verify**
   - Direct field access should work
   - All keyword fields should be extractable

## Implementation Timeline

- ✅ Phase 1: Parser support for keywords as identifiers (DONE)
- ⏳ Phase 2: Codegen support for field extraction (IN PROGRESS)
- ⏳ Phase 3: Testing and validation (PENDING)

## Related Features

This work builds on:
- Mutable variable support (mut keyword)
- Extended C type system
- External function bindings (extern fn)

## Questions?

Refer to the detailed documents:
- For implementation details → KEYWORDS_FIELDS_IMPLEMENTATION_REPORT.md
- For usage patterns → DOT_ACCESSOR_KEYWORDS.md
- For architecture → KEYWORDS_AS_FIELDS_STATUS.md
- For testing → KEYWORDS_AS_FIELDS_COMPLETE.md

## Files Affected

Core changes:
- `src/parser.cpp` - 45 lines modified

No changes to:
- `src/lexer.h`, `src/lexer.cpp` - keywords already defined
- `src/ast.h` - no new AST nodes needed
- `src/llvm_codegen.cpp` - (yet - coming in Phase 2)

Example files created:
- `examples/sdl_field_access_test.ct`
- `examples/simple_keyword_field_test.ct`

Documentation files created:
- This index document
- 4 comprehensive guides
- Status report
- Implementation report
