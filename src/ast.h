#ifndef CONSTRUCT_AST_H
#define CONSTRUCT_AST_H

#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <optional>

namespace construct {

// Forward declarations
struct Type;
struct Expr;
struct Pattern;
struct Statement;

using TypePtr = std::shared_ptr<Type>;
using ExprPtr = std::shared_ptr<Expr>;
using PatternPtr = std::shared_ptr<Pattern>;
using StmtPtr = std::shared_ptr<Statement>;

// ============================================================================
// TYPE SYSTEM
// ============================================================================

struct Type {
    enum Kind {
        // Primitive integer types
        INT,           // int (32-bit)
        CHAR,          // char (8-bit signed)
        UCHAR,         // unsigned char (8-bit unsigned)
        SHORT,         // short (16-bit signed)
        USHORT,        // unsigned short (16-bit unsigned)
        UINT,          // unsigned int (32-bit unsigned)
        LONG,          // long (64-bit signed)
        ULONG,         // unsigned long (64-bit unsigned)
        LONGLONG,      // long long (64-bit signed)
        ULONGLONG,     // unsigned long long (64-bit unsigned)
        
        // Primitive float types
        FLOAT,         // float (32-bit)
        DOUBLE,        // double (64-bit)
        BOOL,
        STRING,
        
        // FFI/C interop types
        CSTR,          // C string (const char*)
        INTPTR,        // Raw pointer (i8*)
        VOID,          // Void return type
        
        // Composite types
        FUNCTION,      // a -> b
        LIST,          // [a]
        TUPLE,         // (a, b, c)
        RECORD,        // {x: Int, y: Int}
        
        // Meta types
        VARIABLE,      // Type variable (for inference)
        UNKNOWN,
    } kind;

    // For FUNCTION: paramType -> returnType
    TypePtr param_type;
    TypePtr return_type;

    // For LIST: element type
    TypePtr element_type;

    // For TUPLE: element types
    std::vector<TypePtr> element_types;

    // For RECORD: field name -> field type
    std::vector<std::pair<std::string, TypePtr>> fields;

    // For VARIABLE: variable name (e.g., "'a", "'b")
    std::string var_name;

    // Source location for error reporting
    int line = 0;
    int column = 0;

    Type(Kind k) : kind(k) {}
    
    // Integer type factories
    static TypePtr makeInt() {
        return std::make_shared<Type>(INT);
    }
    static TypePtr makeChar() {
        return std::make_shared<Type>(CHAR);
    }
    static TypePtr makeUChar() {
        return std::make_shared<Type>(UCHAR);
    }
    static TypePtr makeShort() {
        return std::make_shared<Type>(SHORT);
    }
    static TypePtr makeUShort() {
        return std::make_shared<Type>(USHORT);
    }
    static TypePtr makeUInt() {
        return std::make_shared<Type>(UINT);
    }
    static TypePtr makeLong() {
        return std::make_shared<Type>(LONG);
    }
    static TypePtr makeULong() {
        return std::make_shared<Type>(ULONG);
    }
    static TypePtr makeLongLong() {
        return std::make_shared<Type>(LONGLONG);
    }
    static TypePtr makeULongLong() {
        return std::make_shared<Type>(ULONGLONG);
    }
    
    // Float type factories
    static TypePtr makeFloat() {
        return std::make_shared<Type>(FLOAT);
    }
    static TypePtr makeDouble() {
        return std::make_shared<Type>(DOUBLE);
    }
    
    // Other type factories
    static TypePtr makeBool() {
        return std::make_shared<Type>(BOOL);
    }
    static TypePtr makeString() {
        return std::make_shared<Type>(STRING);
    }
    static TypePtr makeCStr() {
        return std::make_shared<Type>(CSTR);
    }
    static TypePtr makeIntPtr() {
        return std::make_shared<Type>(INTPTR);
    }
    static TypePtr makeVoid() {
        return std::make_shared<Type>(VOID);
    }
    static TypePtr makeFunction(TypePtr param, TypePtr ret) {
        auto t = std::make_shared<Type>(FUNCTION);
        t->param_type = param;
        t->return_type = ret;
        return t;
    }
    static TypePtr makeList(TypePtr elem) {
        auto t = std::make_shared<Type>(LIST);
        t->element_type = elem;
        return t;
    }
    static TypePtr makeTuple(const std::vector<TypePtr>& elems) {
        auto t = std::make_shared<Type>(TUPLE);
        t->element_types = elems;
        return t;
    }
};

// ============================================================================
// PATTERNS (for pattern matching and function parameters)
// ============================================================================

struct Pattern {
    enum Kind {
        WILDCARD,      // _
        IDENTIFIER,    // x
        LITERAL,       // 0, "str", true
        CONSTRUCTOR,   // Some(x), None
        TUPLE,         // (a, b)
        LIST,          // [x, y, ...rest]
    } kind;

    // For IDENTIFIER and CONSTRUCTOR
    std::string name;
    
    // For LITERAL: the literal value
    std::string value;
    
    // For CONSTRUCTOR and TUPLE: sub-patterns
    std::vector<PatternPtr> subpatterns;

    // For LIST: patterns and optional rest
    std::vector<PatternPtr> elements;
    std::optional<std::string> rest_binding;

    int line = 0;
    int column = 0;

    Pattern(Kind k) : kind(k) {}
};

// ============================================================================
// EXPRESSIONS
// ============================================================================

struct Expr {
    enum Kind {
        // Literals
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        BOOL_LITERAL,
        
        // Variables and identifiers
        IDENTIFIER,
        
        // Operators and operations
        BINARY_OP,     // a + b, a |> b
        UNARY_OP,      // -x, !x
        APPLICATION,   // f(x) or f x
        
        // Control flow
        IF_EXPR,       // if cond then a else b
        MATCH_EXPR,    // match value: | pattern -> expr
        FOR_LOOP,      // for x in iterable do body
        WHILE_LOOP,    // while condition do body
        
        // Functions
        LAMBDA,        // fn (x: Int) -> x + 1
        BLOCK_FN,      // fn example(x: Int): Int { ... }
        
        // Collections
        LIST_LITERAL,  // [1, 2, 3]
        LIST_COMP,     // [x * 2 for x in xs]
        RECORD_LITERAL,// {x: 10, y: 20}
        TUPLE_LITERAL, // (a, b, c)
        
        // Record operations
        FIELD_ACCESS,  // record.field
        RECORD_UPDATE, // record edit {field: value}
        
        // Assignment (for mutable variables)
        ASSIGNMENT,    // x = value (only for mut variables)
        
        // Other
        PIPE,          // a |> f
        BLOCK,         // sequential statements
    } kind;

    // Literals
    std::string literal_value;
    
    // Identifiers
    std::string name;
    
    // Binary operations
    std::string op;
    ExprPtr left;
    ExprPtr right;
    
    // Unary operations
    ExprPtr operand;
    
    // Function application
    ExprPtr function_expr;
    std::vector<ExprPtr> arguments;
    
    // If expression
    ExprPtr condition;
    ExprPtr then_expr;
    ExprPtr else_expr;
    
    // Match expression
    struct MatchCase {
        PatternPtr pattern;
        ExprPtr body;
    };
    ExprPtr match_value;
    std::vector<MatchCase> cases;
    
    // For loop: for var in iterable do loop_body
    std::string loop_var;
    ExprPtr iterable;
    ExprPtr loop_body;  // The body of the for loop
    
    // While loop: while condition do loop_body
    // Uses condition and loop_body fields
    
    // Lambda / Function definition
    struct Parameter {
        std::string name;
        TypePtr type;  // optional
    };
    std::vector<Parameter> parameters;
    ExprPtr body;  // The body of the function
    std::string fn_name;  // for named functions
    TypePtr return_type;  // optional
    bool is_extern = false;  // Mark external/foreign functions
    
    // For BLOCK: sequence of statements
    std::vector<StmtPtr> statements;
    
    // For LIST_LITERAL and TUPLE_LITERAL: sequence of expressions
    std::vector<ExprPtr> elements;
    
    // List comprehension
    struct ComprehensionBinding {
        std::string var_name;
        ExprPtr iterable;
    };
    ExprPtr comp_expr;
    std::vector<ComprehensionBinding> comp_bindings;
    
    // Record
    std::vector<std::pair<std::string, ExprPtr>> record_fields;
    
    // Field access
    std::string field_name;
    ExprPtr record_expr;
    
    // Record update
    ExprPtr base_record;
    std::vector<std::pair<std::string, ExprPtr>> update_fields;

    // Type annotation (for let bindings, function returns)
    TypePtr expr_type;

    int line = 0;
    int column = 0;

    Expr(Kind k) : kind(k) {}
};

// ============================================================================
// STATEMENTS (top-level)
// ============================================================================

struct Statement {
    enum Kind {
        EXPR_STMT,        // expression as statement
        LET_BINDING,      // x: Int = expr
        FUNCTION_DEF,     // function definition
        TYPE_DEF,         // type alias
        EXTERN_TYPE_DEF,  // extern type definition (for C struct interop)
        IMPORT,           // import statement
        INCLUDE,          // include statement
    } kind;

    ExprPtr expr;
    std::string name;
    TypePtr type_annotation;
    bool is_mutable = false;  // For mutable variables (mut keyword)
    
    // Type definition
    TypePtr type_value;
    
    // Extern type definition: struct fields {name: type, ...}
    std::vector<std::pair<std::string, TypePtr>> struct_fields;

    // Import
    std::string module_name;
    
    // Include: file paths or patterns (e.g., ["lib/*.ct"], ["*"] for wildcard)
    std::vector<std::string> include_paths;

    int line = 0;
    int column = 0;

    Statement(Kind k) : kind(k) {}
};

// ============================================================================
// PROGRAM (collection of statements)
// ============================================================================

struct Program {
    std::vector<StmtPtr> statements;
};

}  // namespace construct

#endif // CONSTRUCT_AST_H
