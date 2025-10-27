#include "lexer.h"
#include <cctype>
#include <stdexcept>

namespace construct {

const std::unordered_map<std::string, TokenType> Lexer::KEYWORDS = {
    {"if", TokenType::KW_IF},
    {"then", TokenType::KW_THEN},
    {"else", TokenType::KW_ELSE},
    {"match", TokenType::KW_MATCH},
    {"fn", TokenType::KW_FN},
    {"let", TokenType::KW_LET},
    {"type", TokenType::KW_TYPE},
    {"import", TokenType::KW_IMPORT},
    {"include", TokenType::KW_INCLUDE},
    {"true", TokenType::KW_TRUE},
    {"false", TokenType::KW_FALSE},
    {"for", TokenType::KW_FOR},
    {"in", TokenType::KW_IN},
    {"return", TokenType::KW_RETURN},
    {"do", TokenType::KW_DO},
    {"end", TokenType::KW_END},
    {"while", TokenType::KW_WHILE},
    {"extern", TokenType::KW_EXTERN},
    {"edit", TokenType::EDIT},
};

Lexer::Lexer(const std::string& source) : source(source) {}

char Lexer::current() const {
    if (position >= source.length()) return '\0';
    return source[position];
}

char Lexer::peek(int offset) const {
    size_t pos = position + offset;
    if (pos >= source.length()) return '\0';
    return source[pos];
}

void Lexer::advance() {
    if (position < source.length()) {
        if (source[position] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        position++;
    }
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    return Token(type, value, line, column - static_cast<int>(value.length()));
}

void Lexer::skipWhitespace() {
    while (current() == ' ' || current() == '\t') {
        advance();
    }
}

void Lexer::skipComment() {
    // Handle # comments
    if (current() == '#') {
        while (current() != '\n' && current() != '\0') {
            advance();
        }
    }
    // Handle // comments
    else if (current() == '/' && peek() == '/') {
        advance();  // skip first /
        advance();  // skip second /
        while (current() != '\n' && current() != '\0') {
            advance();
        }
    }
    // Handle /* */ comments
    else if (current() == '/' && peek() == '*') {
        advance();  // skip /
        advance();  // skip *
        while (!(current() == '*' && peek() == '/') && current() != '\0') {
            advance();
        }
        if (current() == '*') advance();  // skip *
        if (current() == '/') advance();  // skip /
    }
}

Token Lexer::readNumber() {
    int start_line = line;
    int start_col = column;
    std::string num;
    
    while (std::isdigit(current())) {
        num += current();
        advance();
    }
    
    if (current() == '.' && std::isdigit(peek())) {
        num += current();
        advance();
        while (std::isdigit(current())) {
            num += current();
            advance();
        }
        return Token(TokenType::FLOAT, num, start_line, start_col);
    }
    
    return Token(TokenType::INT, num, start_line, start_col);
}

Token Lexer::readString(char quote) {
    int start_line = line;
    int start_col = column;
    std::string str;
    advance();  // skip opening quote
    
    while (current() != quote && current() != '\0') {
        // Character literals (single quotes) cannot contain newlines
        if (quote == '\'' && current() == '\n') {
            throw std::runtime_error("Unexpected newline in character literal at line " + std::to_string(line));
        }
        
        if (current() == '\\') {
            advance();
            if (current() == 'n') str += '\n';
            else if (current() == 't') str += '\t';
            else if (current() == 'r') str += '\r';
            else if (current() == '\\') str += '\\';
            else if (current() == quote) str += quote;
            else str += current();
            advance();
        } else {
            str += current();
            advance();
        }
    }
    
    if (current() == quote) {
        advance();  // skip closing quote
    } else {
        throw std::runtime_error("Unterminated string at " + std::to_string(line));
    }
    
    return Token(TokenType::STRING, str, start_line, start_col);
}

Token Lexer::readIdentifierOrKeyword() {
    int start_line = line;
    int start_col = column;
    std::string ident;
    
    while (std::isalnum(current()) || current() == '_' || current() == '\'') {
        ident += current();
        advance();
    }
    
    auto it = KEYWORDS.find(ident);
    if (it != KEYWORDS.end()) {
        return Token(it->second, ident, start_line, start_col);
    }
    
    return Token(TokenType::IDENTIFIER, ident, start_line, start_col);
}

void Lexer::handleIndentation() {
    if (position == 0 || source[position - 1] == '\n') {
        int indent = 0;
        while (current() == ' ' || current() == '\t') {
            if (current() == ' ') indent++;
            else indent += 4;  // treat tab as 4 spaces
            advance();
        }
        
        // Skip blank lines and comments
        if (current() == '\n' || current() == '#') {
            skipComment();
            if (current() == '\n') advance();
            handleIndentation();
            return;
        }
        
        // Skip indentation processing if we're in a continuation context
        // (inside parentheses/brackets or after an arrow)
        if (paren_depth > 0 || last_token_is_continuation) {
            last_token_is_continuation = false;  // Reset the flag
            return;
        }
        
        int current_indent = indent_stack.back();
        if (indent > current_indent) {
            indent_stack.push_back(indent);
            pending_tokens.push_back(Token(TokenType::INDENT, "", line, column));
        } else if (indent < current_indent) {
            while (!indent_stack.empty() && indent_stack.back() > indent) {
                indent_stack.pop_back();
                pending_tokens.push_back(Token(TokenType::DEDENT, "", line, column));
            }
        }
    }
}

Token Lexer::nextToken() {
    // Return pending tokens first
    if (!pending_tokens.empty()) {
        Token t = pending_tokens.front();
        pending_tokens.erase(pending_tokens.begin());
        return t;
    }
    
    skipWhitespace();
    skipComment();
    
    // Handle indentation-sensitive newlines
    if (current() == '\n') {
        int start_line = line;
        int start_col = column;
        advance();
        handleIndentation();
        if (!pending_tokens.empty()) {
            Token t = pending_tokens.front();
            pending_tokens.erase(pending_tokens.begin());
            return t;
        }
        return Token(TokenType::NEWLINE, "\n", start_line, start_col);
    }
    
    if (current() == '\0') {
        // Emit remaining dedents at EOF
        while (indent_stack.size() > 1) {
            indent_stack.pop_back();
            return Token(TokenType::DEDENT, "", line, column);
        }
        return Token(TokenType::EOF_TOKEN, "", line, column);
    }
    
    // Single character tokens
    if (current() == '(') {
        advance();
        paren_depth++;
        return makeToken(TokenType::LPAREN, "(");
    }
    if (current() == ')') {
        advance();
        paren_depth--;
        return makeToken(TokenType::RPAREN, ")");
    }
    if (current() == '{') {
        advance();
        paren_depth++;
        return makeToken(TokenType::LBRACE, "{");
    }
    if (current() == '}') {
        advance();
        paren_depth--;
        return makeToken(TokenType::RBRACE, "}");
    }
    if (current() == '[') {
        advance();
        paren_depth++;
        return makeToken(TokenType::LBRACKET, "[");
    }
    if (current() == ']') {
        advance();
        paren_depth--;
        return makeToken(TokenType::RBRACKET, "]");
    }
    if (current() == ',') {
        advance();
        return makeToken(TokenType::COMMA, ",");
    }
    if (current() == ';') {
        advance();
        return makeToken(TokenType::SEMICOLON, ";");
    }
    
    // Operators and multi-character tokens
    if (current() == ':') {
        advance();
        return makeToken(TokenType::COLON, ":");
    }
    
    if (current() == '.') {
        advance();
        return makeToken(TokenType::DOT, ".");
    }
    
    if (current() == '+') {
        advance();
        return makeToken(TokenType::PLUS, "+");
    }
    
    if (current() == '-') {
        advance();
        if (current() == '>') {
            advance();
            last_token_is_continuation = true;  // Mark that next line is a continuation
            return makeToken(TokenType::ARROW, "->");
        }
        return makeToken(TokenType::MINUS, "-");
    }
    
    if (current() == '*') {
        advance();
        if (current() == '*') {
            advance();
            return makeToken(TokenType::POWER, "**");
        }
        return makeToken(TokenType::STAR, "*");
    }
    
    if (current() == '/') {
        // Check for comments
        if (peek() == '/' || peek() == '*') {
            skipComment();
            return nextToken();  // Recursively get the next token
        }
        advance();
        return makeToken(TokenType::SLASH, "/");
    }
    
    if (current() == '%') {
        advance();
        return makeToken(TokenType::PERCENT, "%");
    }
    
    if (current() == '|') {
        advance();
        if (current() == '>') {
            advance();
            return makeToken(TokenType::PIPE, "|>");
        }
        return makeToken(TokenType::PIPE_MATCH, "|");
    }
    
    if (current() == '=') {
        advance();
        if (current() == '=') {
            advance();
            return makeToken(TokenType::EQ_EQ, "==");
        }
        return makeToken(TokenType::EQ, "=");
    }
    
    if (current() == '!') {
        advance();
        if (current() == '=') {
            advance();
            return makeToken(TokenType::NOT_EQ, "!=");
        }
        return makeToken(TokenType::NOT, "!");
    }
    
    if (current() == '<') {
        advance();
        if (current() == '=') {
            advance();
            return makeToken(TokenType::LE, "<=");
        }
        return makeToken(TokenType::LT, "<");
    }
    
    if (current() == '>') {
        advance();
        if (current() == '=') {
            advance();
            return makeToken(TokenType::GE, ">=");
        }
        return makeToken(TokenType::GT, ">");
    }
    
    if (current() == '&') {
        advance();
        if (current() == '&') {
            advance();
            return makeToken(TokenType::AND, "&&");
        }
        return Token(TokenType::ERROR, "&", line, column);
    }
    
    if (current() == '"' || current() == '\'') {
        char quote = current();
        return readString(quote);
    }
    
    // Numbers
    if (std::isdigit(current())) {
        return readNumber();
    }
    
    // Identifiers and keywords
    if (std::isalpha(current()) || current() == '_') {
        return readIdentifierOrKeyword();
    }
    
    // Unknown character
    char c = current();
    advance();
    return Token(TokenType::ERROR, std::string(1, c), line, column);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token t = nextToken();
    while (t.type != TokenType::EOF_TOKEN) {
        tokens.push_back(t);
        t = nextToken();
    }
    tokens.push_back(t);  // Add EOF token
    return tokens;
}

}  // namespace construct
