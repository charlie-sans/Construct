#ifndef CONSTRUCT_LEXER_H
#define CONSTRUCT_LEXER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace construct {

// ============================================================================
// TOKEN TYPES
// ============================================================================

enum class TokenType {
    // Literals
    INT,
    FLOAT,
    STRING,
    IDENTIFIER,
    
    // Keywords
    KW_IF,
    KW_THEN,
    KW_ELSE,
    KW_MATCH,
    KW_FN,
    KW_LET,
    KW_TYPE,
    KW_IMPORT,
    KW_TRUE,
    KW_FALSE,
    KW_FOR,
    KW_IN,
    KW_RETURN,
    KW_DO,
    KW_END,
    KW_WHILE,
    
    // Operators
    PLUS,          // +
    MINUS,         // -
    STAR,          // *
    SLASH,         // /
    PERCENT,       // %
    POWER,         // **
    PIPE,          // |>
    PIPE_MATCH,    // | (in match)
    EQ,            // =
    EQ_EQ,         // ==
    NOT_EQ,        // !=
    LT,            // <
    LE,            // <=
    GT,            // >
    GE,            // >=
    AND,           // &&
    OR,            // ||
    NOT,           // !
    ARROW,         // ->
    EDIT,          // edit (for record update)
    DOT,           // .
    COLON,         // :
    COMMA,         // ,
    SEMICOLON,     // ;
    
    // Delimiters
    LPAREN,        // (
    RPAREN,        // )
    LBRACE,        // {
    RBRACE,        // }
    LBRACKET,      // [
    RBRACKET,      // ]
    
    // Special
    EOF_TOKEN,
    NEWLINE,
    INDENT,
    DEDENT,
    ERROR,
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t, const std::string& v, int l, int c)
        : type(t), value(v), line(l), column(c) {}
};

// ============================================================================
// LEXER
// ============================================================================

class Lexer {
public:
    Lexer(const std::string& source);
    
    std::vector<Token> tokenize();
    Token nextToken();
    
private:
    std::string source;
    size_t position = 0;
    int line = 1;
    int column = 1;
    std::vector<int> indent_stack = {0};
    bool pending_dedents = false;
    std::vector<Token> pending_tokens;
    
    static const std::unordered_map<std::string, TokenType> KEYWORDS;
    
    // Helper methods
    char current() const;
    char peek(int offset = 1) const;
    void advance();
    void skipWhitespace();
    void skipComment();
    
    Token makeToken(TokenType type, const std::string& value);
    Token readNumber();
    Token readString(char quote);
    Token readIdentifierOrKeyword();
    
    // Indentation handling
    void handleIndentation();
    Token makeDedent();
};

}  // namespace construct

#endif // CONSTRUCT_LEXER_H
