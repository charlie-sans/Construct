#ifndef CONSTRUCT_PARSER_H
#define CONSTRUCT_PARSER_H

#include "ast.h"
#include "lexer.h"
#include <vector>
#include <memory>
#include <stdexcept>

namespace construct {

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    
    Program parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;
    
    // Navigation
    Token peek(int offset = 0) const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    Token consume(TokenType type, const std::string& message);
    void synchronize();
    
    // Parsing methods
    StmtPtr parseStatement();
    StmtPtr parseLetBinding();
    StmtPtr parseFunctionDef();
    ExprPtr parseExpression();
    ExprPtr parsePipe();
    ExprPtr parseLogicalOr();
    ExprPtr parseLogicalAnd();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseAdditive();
    ExprPtr parseMultiplicative();
    ExprPtr parseUnary();
    ExprPtr parsePower();
    ExprPtr parsePostfix();
    ExprPtr parsePrimary();

    bool isVisible(const std::string &str);

    // Helper parsers
    TypePtr parseType();
    TypePtr parseFunctionType();
    PatternPtr parsePattern();
    ExprPtr parseIfExpr();
    ExprPtr parseMatchExpr();
    ExprPtr parseForLoop();
    ExprPtr parseWhileLoop();
    ExprPtr parseLambda();
    ExprPtr parseBlockFunction();
    ExprPtr parseList();
    ExprPtr parseListComprehension(const std::vector<ExprPtr>& initial);
    ExprPtr parseRecord();
    ExprPtr parseRecordUpdate(ExprPtr base);
    std::vector<std::pair<std::string, ExprPtr>> parseRecordFields();
    std::vector<Expr::Parameter> parseFunctionParameters();
    std::vector<Expr::MatchCase> parseMatchCases();
};

}  // namespace construct

#endif // CONSTRUCT_PARSER_H
