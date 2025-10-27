#include "parser.h"
#include <sstream>
#include <iostream>

namespace construct {

// Helper function to convert TokenType to string
static std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::STRING: return "STRING";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::KW_IF: return "IF";
        case TokenType::KW_THEN: return "THEN";
        case TokenType::KW_ELSE: return "ELSE";
        case TokenType::KW_MATCH: return "MATCH";
        case TokenType::KW_FN: return "FN";
        case TokenType::KW_LET: return "LET";
        case TokenType::KW_TYPE: return "TYPE";
        case TokenType::KW_IMPORT: return "IMPORT";
        case TokenType::KW_TRUE: return "TRUE";
        case TokenType::KW_FALSE: return "FALSE";
        case TokenType::KW_FOR: return "FOR";
        case TokenType::KW_IN: return "IN";
        case TokenType::KW_RETURN: return "RETURN";
        case TokenType::KW_DO: return "DO";
        case TokenType::KW_END: return "END";
        case TokenType::KW_WHILE: return "WHILE";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::PERCENT: return "%";
        case TokenType::POWER: return "**";
        case TokenType::PIPE: return "|>";
        case TokenType::PIPE_MATCH: return "|";
        case TokenType::EQ: return "=";
        case TokenType::EQ_EQ: return "==";
        case TokenType::NOT_EQ: return "!=";
        case TokenType::LT: return "<";
        case TokenType::LE: return "<=";
        case TokenType::GT: return ">";
        case TokenType::GE: return ">=";
        case TokenType::AND: return "&&";
        case TokenType::OR: return "||";
        case TokenType::NOT: return "!";
        case TokenType::ARROW: return "->";
        case TokenType::EDIT: return "edit";
        case TokenType::DOT: return ".";
        case TokenType::COLON: return ":";
        case TokenType::COMMA: return ",";
        case TokenType::SEMICOLON: return ";";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::LBRACKET: return "[";
        case TokenType::RBRACKET: return "]";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::INDENT: return "INDENT";
        case TokenType::DEDENT: return "DEDENT";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Helper function to format token value with escaping
static std::string formatTokenValue(const std::string& value) {
    std::ostringstream oss;
    oss << "'";
    for (char c : value) {
        if (c >= 32 && c <= 126) {
            if (c == '\'') oss << "\\'";
            else if (c == '\\') oss << "\\\\";
            else oss << c;
        } else {
            oss << "\\x" << std::hex << (static_cast<unsigned char>(c) >> 4) 
                << (static_cast<unsigned char>(c) & 0xf);
        }
    }
    oss << "'";
    return oss.str();
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

Token Parser::peek(int offset) const {
    size_t pos = current + offset;
    if (pos >= tokens.size()) {
        return tokens.back();  // Return EOF
    }
    return tokens[pos];
}

Token Parser::advance() {
    Token t = peek();
    if (current < tokens.size()) current++;
    return t;
}

bool Parser::check(TokenType type) const {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    // Skip any newlines and indentation before checking for the expected token
    while (check(TokenType::NEWLINE) || check(TokenType::INDENT) || check(TokenType::DEDENT)) {
        advance();
    }
    
    if (check(type)) {
        return advance();
    }
    Token cur = peek();
    std::ostringstream oss;
    oss << message << " at line " << cur.line << ", column " << cur.column 
        << ". Got '" << tokenTypeToString(cur.type) << "'";
    if (!cur.value.empty()) {
        oss << " with value " << formatTokenValue(cur.value);
    }
    throw ParseError(oss.str());
}

void Parser::synchronize() {
    advance();
    while (!check(TokenType::EOF_TOKEN)) {
        if (peek().type == TokenType::KW_LET ||
            peek().type == TokenType::KW_FN ||
            peek().type == TokenType::KW_TYPE) {
            return;
        }
        advance();
    }
}

Program Parser::parse() {
    Program program;
    
    while (!check(TokenType::EOF_TOKEN)) {
        // Skip newlines at statement level
        while (match(TokenType::NEWLINE)) {}

        if (check(TokenType::EOF_TOKEN)) break;

        try {
            auto stmt = parseStatement();
            if (stmt) {
                program.statements.push_back(stmt);
            }
        } catch (const ParseError& e) {
            // Print error to stderr
            std::cerr << "Parse error: " << e.what() << std::endl;
            synchronize();
        }

        while (match(TokenType::NEWLINE)) {}
    }
    
    return program;
}

StmtPtr Parser::parseStatement() {
    // Skip any newlines and indentation
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    if (check(TokenType::KW_LET)) {
        return parseLetBinding();
    }
    
    if (check(TokenType::KW_FN)) {
        return parseFunctionDef();
    }
    
    if (check(TokenType::KW_TYPE)) {
        advance();  // skip 'type'
        auto stmt = std::make_shared<Statement>(Statement::TYPE_DEF);
        stmt->name = consume(TokenType::IDENTIFIER, "Expected type name").value;
        consume(TokenType::EQ, "Expected '=' in type definition");
        stmt->type_value = parseType();
        return stmt;
    }
    
    if (check(TokenType::KW_IMPORT)) {
        advance();  // skip 'import'
        auto stmt = std::make_shared<Statement>(Statement::IMPORT);
        stmt->module_name = consume(TokenType::IDENTIFIER, "Expected module name").value;
        return stmt;
    }
    
    // Allow expression statements at top level (e.g., dump(x))
    // This enables side-effecting expressions like function calls
    try {
        auto expr = parseExpression();
        auto stmt = std::make_shared<Statement>(Statement::EXPR_STMT);
        stmt->expr = expr;
        return stmt;
    } catch (const ParseError&) {
        // If expression parsing fails, throw a more informative error
        Token cur = peek();
        std::ostringstream oss;
        oss << "Top-level statement must start with 'let', 'fn', 'type', 'import', or an expression "
            << "at line " << cur.line << ", column " << cur.column 
            << ". Got '" << tokenTypeToString(cur.type) << "'";
        if (!cur.value.empty()) {
            oss << " with value " << formatTokenValue(cur.value);
        }
        throw ParseError(oss.str());
    }
}

StmtPtr Parser::parseLetBinding() {
    auto stmt = std::make_shared<Statement>(Statement::LET_BINDING);
    advance();  // consume 'let'

    stmt->name = consume(TokenType::IDENTIFIER, "Expected identifier").value;

    // Optional type annotation before =
    if (match(TokenType::COLON)) {
        stmt->type_annotation = parseType();
    }

    // Accept optional type annotation after =
    consume(TokenType::EQ, "Expected '=' in let binding");
    if (match(TokenType::COLON)) {
        stmt->type_annotation = parseType();
    }

    stmt->expr = parseExpression();
    return stmt;
}

StmtPtr Parser::parseFunctionDef() {
    auto stmt = std::make_shared<Statement>(Statement::EXPR_STMT);
    auto expr = std::make_shared<Expr>(Expr::LAMBDA);

    advance();  // consume 'fn'

    // Check for block-style function: fn name(params): type { ... } or fn name(params) -> type { ... }
    // or simple-style: fn name(params) = expr
    if (check(TokenType::IDENTIFIER)) {
        auto nameToken = peek();
        if (tokens[current + 1].type == TokenType::LPAREN) {
            // Named function (block or simple style)
            expr->fn_name = advance().value;  // consume name

            advance();  // consume '('
            expr->parameters = parseFunctionParameters();
            consume(TokenType::RPAREN, "Expected ')' after parameters");

            // Check for simple-style: = expr
            if (match(TokenType::EQ)) {
                expr->kind = Expr::LAMBDA;
                expr->body = parseExpression();
                stmt->expr = expr;
                return stmt;
            }

            // Block style: fn name(params) { ... } or fn name(params) -> type { ... }
            expr->kind = Expr::BLOCK_FN;

            // Accept either : type or -> type
            if (match(TokenType::COLON) || match(TokenType::ARROW)) {
                expr->return_type = parseType();
            }

            consume(TokenType::LBRACE, "Expected '{' for function body");

            // Parse function body (list of statements)
            auto block = std::make_shared<Expr>(Expr::BLOCK);
            block->elements.clear();

            while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
                while (match(TokenType::NEWLINE)) {}
                if (check(TokenType::RBRACE)) break;

                if (match(TokenType::KW_RETURN)) {
                    auto ret_expr = std::make_shared<Expr>(Expr::IF_EXPR);
                    ret_expr->kind = Expr::IDENTIFIER;
                    ret_expr->name = "return";
                    ret_expr->then_expr = parseExpression();
                    block->elements.push_back(ret_expr);
                } else {
                    block->elements.push_back(parseExpression());
                }

                while (match(TokenType::NEWLINE)) {}
            }

            consume(TokenType::RBRACE, "Expected '}' after function body");
            expr->body = block;
            stmt->expr = expr;
            return stmt;
        }
    }

    // Expression-style lambda: fn (x: Int) -> x + 1
    advance();  // skip extra 'fn' if present - handled above

    if (match(TokenType::LPAREN)) {
        expr->parameters = parseFunctionParameters();
        consume(TokenType::RPAREN, "Expected ')' after parameters");
    }

    // Accept either : type or -> type for return type
    if (match(TokenType::COLON) || match(TokenType::ARROW)) {
        expr->return_type = parseType();
    }

    expr->body = parseExpression();

    stmt->expr = expr;
    return stmt;
}

std::vector<Expr::Parameter> Parser::parseFunctionParameters() {
    std::vector<Expr::Parameter> params;
    
    if (!check(TokenType::RPAREN)) {
        do {
            Expr::Parameter param;
            param.name = consume(TokenType::IDENTIFIER, "Expected parameter name").value;
            
            if (match(TokenType::COLON)) {
                param.type = parseType();
            }
            
            params.push_back(param);
        } while (match(TokenType::COMMA));
    }
    
    return params;
}

TypePtr Parser::parseType() {
    return parseFunctionType();
}

TypePtr Parser::parseFunctionType() {
    auto left = std::make_shared<Type>(Type::UNKNOWN);
    
    if (match(TokenType::LBRACKET)) {
        left = std::make_shared<Type>(Type::LIST);
        left->element_type = parseType();
        consume(TokenType::RBRACKET, "Expected ']'");
    } else if (match(TokenType::LBRACE)) {
        left = std::make_shared<Type>(Type::RECORD);
        // Parse record fields
        do {
            std::string field = consume(TokenType::IDENTIFIER, "Expected field name").value;
            consume(TokenType::COLON, "Expected ':' in record type");
            auto field_type = parseType();
            left->fields.push_back({field, field_type});
        } while (match(TokenType::COMMA));
        consume(TokenType::RBRACE, "Expected '}'");
    } else if (match(TokenType::LPAREN)) {
        left = std::make_shared<Type>(Type::TUPLE);
        do {
            left->element_types.push_back(parseType());
        } while (match(TokenType::COMMA));
        consume(TokenType::RPAREN, "Expected ')'");
    } else {
        Token t = advance();
        if (t.type == TokenType::IDENTIFIER) {
            if (t.value == "Int") left = Type::makeInt();
            else if (t.value == "Float") left = Type::makeFloat();
            else if (t.value == "Bool") left = Type::makeBool();
            else if (t.value == "String") left = Type::makeString();
            else {
                left = std::make_shared<Type>(Type::VARIABLE);
                left->var_name = t.value;
            }
        } else {
            throw ParseError("Expected type");
        }
    }
    
    // Handle function types: a -> b
    if (match(TokenType::ARROW)) {
        auto right = parseFunctionType();
        return Type::makeFunction(left, right);
    }
    
    return left;
}

ExprPtr Parser::parseExpression() {
    return parsePipe();
}

ExprPtr Parser::parsePipe() {
    auto expr = parseLogicalOr();
    
    while (match(TokenType::PIPE)) {
        auto right = parseLogicalOr();
        auto pipe_expr = std::make_shared<Expr>(Expr::PIPE);
        pipe_expr->left = expr;
        pipe_expr->right = right;
        expr = pipe_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseLogicalOr() {
    auto expr = parseLogicalAnd();
    
    while (check(TokenType::OR)) {
        advance();
        auto right = parseLogicalAnd();
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = "||";
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseLogicalAnd() {
    auto expr = parseEquality();
    
    while (check(TokenType::AND)) {
        advance();
        auto right = parseEquality();
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = "&&";
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseEquality() {
    auto expr = parseComparison();
    
    while (check(TokenType::EQ_EQ) || check(TokenType::NOT_EQ)) {
        advance();
        Token op = tokens[current - 1];
        auto right = parseComparison();
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = op.value;
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseComparison() {
    auto expr = parseAdditive();
    
    while (check(TokenType::LT) || check(TokenType::LE) || 
           check(TokenType::GT) || check(TokenType::GE)) {
        advance();
        Token op = tokens[current - 1];
        auto right = parseAdditive();
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = op.value;
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseAdditive() {
    auto expr = parseMultiplicative();
    
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        advance();
        Token op = tokens[current - 1];
        auto right = parseMultiplicative();
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = op.value;
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseMultiplicative() {
    auto expr = parsePower();
    
    while (check(TokenType::STAR) || check(TokenType::SLASH) || check(TokenType::PERCENT)) {
        advance();
        Token op = tokens[current - 1];
        auto right = parsePower();
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = op.value;
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parsePower() {
    auto expr = parseUnary();
    
    if (match(TokenType::POWER)) {
        auto right = parsePower();  // Right associative
        auto op_expr = std::make_shared<Expr>(Expr::BINARY_OP);
        op_expr->op = "**";
        op_expr->left = expr;
        op_expr->right = right;
        expr = op_expr;
    }
    
    return expr;
}

ExprPtr Parser::parseUnary() {
    if (check(TokenType::MINUS) || check(TokenType::NOT)) {
        advance();
        Token op = tokens[current - 1];
        auto expr = std::make_shared<Expr>(Expr::UNARY_OP);
        expr->op = op.value;
        expr->operand = parseUnary();
        return expr;
    }
    
    return parsePostfix();
}

ExprPtr Parser::parsePostfix() {
    auto expr = parsePrimary();
    
    while (true) {
        if (match(TokenType::LPAREN)) {
            // Function application
            std::vector<ExprPtr> args;
            // Skip whitespace after opening paren
            while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
            if (!check(TokenType::RPAREN)) {
                do {
                    // Skip whitespace before argument
                    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
                    args.push_back(parseExpression());
                    // Skip whitespace after argument
                    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
                } while (match(TokenType::COMMA));
            }
            // Skip whitespace before closing paren
            while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
            consume(TokenType::RPAREN, "Expected ')'");
            
            auto app = std::make_shared<Expr>(Expr::APPLICATION);
            app->function_expr = expr;
            app->arguments = args;
            expr = app;
        } else if (match(TokenType::DOT)) {
            // Field access
            std::string field = consume(TokenType::IDENTIFIER, "Expected field name").value;
            auto access = std::make_shared<Expr>(Expr::FIELD_ACCESS);
            access->record_expr = expr;
            access->field_name = field;
            expr = access;
        } else if (match(TokenType::EDIT)) {
            // Record update
            expr = parseRecordUpdate(expr);
        } else {
            break;
        }
    }
    
    return expr;
}

ExprPtr Parser::parseRecordUpdate(ExprPtr base) {
    consume(TokenType::LBRACE, "Expected '{' after 'edit'");
    auto update = std::make_shared<Expr>(Expr::RECORD_UPDATE);
    update->base_record = base;
    update->update_fields = parseRecordFields();
    consume(TokenType::RBRACE, "Expected '}'");
    return update;
}

ExprPtr Parser::parsePrimary() {
    // If expression
    if (check(TokenType::KW_IF)) {
        return parseIfExpr();
    }
    
    // Match expression
    if (check(TokenType::KW_MATCH)) {
        return parseMatchExpr();
    }
    
    // For loop
    if (check(TokenType::KW_FOR)) {
        return parseForLoop();
    }
    
    // While loop
    if (check(TokenType::KW_WHILE)) {
        return parseWhileLoop();
    }
    
    // Lambda
    if (check(TokenType::KW_FN)) {
        return parseLambda();
    }
    
    // List literal or list comprehension
    if (match(TokenType::LBRACKET)) {
        return parseList();
    }
    
    // Record literal
    if (check(TokenType::LBRACE)) {
        return parseRecord();
    }
    
    // Tuple or grouped expression
    if (match(TokenType::LPAREN)) {
        // Skip whitespace after opening paren
        while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
        
        if (check(TokenType::RPAREN)) {
            advance();
            auto tuple = std::make_shared<Expr>(Expr::TUPLE_LITERAL);
            return tuple;
        }
        
        auto expr = parseExpression();
        
        // Skip whitespace after first expression
        while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
        
        if (match(TokenType::COMMA)) {
            auto tuple = std::make_shared<Expr>(Expr::TUPLE_LITERAL);
            tuple->elements.push_back(expr);
            do {
                // Skip whitespace before next element
                while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
                if (check(TokenType::RPAREN)) break;
                tuple->elements.push_back(parseExpression());
                // Skip whitespace after element
                while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
            } while (match(TokenType::COMMA));
            // Skip whitespace before closing paren
            while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
            consume(TokenType::RPAREN, "Expected ')'");
            return tuple;
        }
        
        // Skip whitespace before closing paren
        while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
        consume(TokenType::RPAREN, "Expected ')'");
        return expr;
    }
    
    // Literals
    if (match(TokenType::INT)) {
        auto lit = std::make_shared<Expr>(Expr::INT_LITERAL);
        lit->literal_value = tokens[current - 1].value;
        return lit;
    }
    
    if (match(TokenType::FLOAT)) {
        auto lit = std::make_shared<Expr>(Expr::FLOAT_LITERAL);
        lit->literal_value = tokens[current - 1].value;
        return lit;
    }
    
    if (match(TokenType::STRING)) {
        auto lit = std::make_shared<Expr>(Expr::STRING_LITERAL);
        lit->literal_value = tokens[current - 1].value;
        return lit;
    }
    
    if (match(TokenType::KW_TRUE)) {
        auto lit = std::make_shared<Expr>(Expr::BOOL_LITERAL);
        lit->literal_value = "true";
        return lit;
    }
    
    if (match(TokenType::KW_FALSE)) {
        auto lit = std::make_shared<Expr>(Expr::BOOL_LITERAL);
        lit->literal_value = "false";
        return lit;
    }
    
    // Identifier
    if (match(TokenType::IDENTIFIER)) {
        auto ident = std::make_shared<Expr>(Expr::IDENTIFIER);
        ident->name = tokens[current - 1].value;
        return ident;
    }
    // Provide detailed error information
    Token cur = peek();
    std::ostringstream oss;
    oss << "Parse error at line " << cur.line << ", column " << cur.column << ": "
        << "Unexpected token '" << tokenTypeToString(cur.type) << "'";
    
    if (!cur.value.empty()) {
        oss << " with value " << formatTokenValue(cur.value);
    }
    
    throw ParseError(oss.str());
}

bool Parser::isVisible(const std::string& str) {
    for (char c : str) {
        if (c < 32 || c > 126) {
            return false;
        }
    }
    return true;
}
ExprPtr Parser::parseIfExpr() {
    auto expr = std::make_shared<Expr>(Expr::IF_EXPR);
    advance();  // consume 'if'
    
    expr->condition = parseExpression();
    
    consume(TokenType::KW_THEN, "Expected 'then'");
    expr->then_expr = parseExpression();
    
    consume(TokenType::KW_ELSE, "Expected 'else'");
    expr->else_expr = parseExpression();
    
    return expr;
}

ExprPtr Parser::parseMatchExpr() {
    auto expr = std::make_shared<Expr>(Expr::MATCH_EXPR);
    advance();  // consume 'match'
    
    expr->match_value = parseExpression();
    consume(TokenType::COLON, "Expected ':' after match value");
    
    expr->cases = parseMatchCases();
    
    return expr;
}

ExprPtr Parser::parseForLoop() {
    auto expr = std::make_shared<Expr>(Expr::FOR_LOOP);
    advance();  // consume 'for'
    
    // Skip whitespace
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    expr->loop_var = consume(TokenType::IDENTIFIER, "Expected loop variable").value;
    consume(TokenType::KW_IN, "Expected 'in' in for loop");
    expr->iterable = parseExpression();
    consume(TokenType::KW_DO, "Expected 'do' in for loop");
    
    // Skip whitespace before body
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    expr->loop_body = parseExpression();
    
    // Skip whitespace before end
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    consume(TokenType::KW_END, "Expected 'end' to close for loop");
    
    return expr;
}

ExprPtr Parser::parseWhileLoop() {
    auto expr = std::make_shared<Expr>(Expr::WHILE_LOOP);
    advance();  // consume 'while'
    
    // Skip whitespace
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    expr->condition = parseExpression();
    consume(TokenType::KW_DO, "Expected 'do' in while loop");
    
    // Skip whitespace before body
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    expr->loop_body = parseExpression();
    
    // Skip whitespace before end
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    consume(TokenType::KW_END, "Expected 'end' to close while loop");
    
    return expr;
}

std::vector<Expr::MatchCase> Parser::parseMatchCases() {
    std::vector<Expr::MatchCase> cases;
    
    while (check(TokenType::PIPE_MATCH)) {
        advance();  // consume '|'
        Expr::MatchCase case_item;
        case_item.pattern = parsePattern();
        consume(TokenType::ARROW, "Expected '->'");
        case_item.body = parseExpression();
        cases.push_back(case_item);
        
        // Optional newline between cases
        while (match(TokenType::NEWLINE)) {}
    }
    
    return cases;
}

PatternPtr Parser::parsePattern() {
    if (match(TokenType::INT)) {
        auto pat = std::make_shared<Pattern>(Pattern::LITERAL);
        pat->value = peek(-1).value;
        return pat;
    }
    
    if (match(TokenType::STRING)) {
        auto pat = std::make_shared<Pattern>(Pattern::LITERAL);
        pat->value = peek(-1).value;
        return pat;
    }
    
    if (match(TokenType::IDENTIFIER)) {
        std::string name = peek(-1).value;
        if (name == "_") {
            return std::make_shared<Pattern>(Pattern::WILDCARD);
        }
        auto pat = std::make_shared<Pattern>(Pattern::IDENTIFIER);
        pat->name = name;
        return pat;
    }
    
    throw ParseError("Expected pattern");
}

ExprPtr Parser::parseLambda() {
    auto expr = std::make_shared<Expr>(Expr::LAMBDA);
    advance();  // consume 'fn'
    
    if (match(TokenType::LPAREN)) {
        expr->parameters = parseFunctionParameters();
        consume(TokenType::RPAREN, "Expected ')'");
    }
    
    consume(TokenType::ARROW, "Expected '->'");
    expr->body = parseExpression();
    
    return expr;
}

ExprPtr Parser::parseList() {
    auto list = std::make_shared<Expr>(Expr::LIST_LITERAL);
    
    // Skip whitespace after opening bracket
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    if (check(TokenType::RBRACKET)) {
        advance();
        return list;
    }
    
    do {
        // Skip whitespace before element
        while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
        list->elements.push_back(parseExpression());
        // Skip whitespace after element
        while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    } while (match(TokenType::COMMA));
    
    // Check for list comprehension
    if (match(TokenType::KW_FOR)) {
        return parseListComprehension(list->elements);
    }
    
    // Skip whitespace before closing bracket
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    consume(TokenType::RBRACKET, "Expected ']'");
    return list;
}

ExprPtr Parser::parseListComprehension(const std::vector<ExprPtr>& initial) {
    auto comp = std::make_shared<Expr>(Expr::LIST_COMP);
    
    if (!initial.empty()) {
        comp->comp_expr = initial[0];
    }
    
    do {
        Expr::ComprehensionBinding binding;
        binding.var_name = consume(TokenType::IDENTIFIER, "Expected variable name").value;
        consume(TokenType::KW_IN, "Expected 'in'");
        binding.iterable = parseExpression();
        comp->comp_bindings.push_back(binding);
    } while (match(TokenType::KW_FOR));
    
    consume(TokenType::RBRACKET, "Expected ']'");
    return comp;
}

ExprPtr Parser::parseRecord() {
    advance();  // consume '{'
    auto record = std::make_shared<Expr>(Expr::RECORD_LITERAL);
    record->record_fields = parseRecordFields();
    consume(TokenType::RBRACE, "Expected '}'");
    return record;
}

std::vector<std::pair<std::string, ExprPtr>> Parser::parseRecordFields() {
    std::vector<std::pair<std::string, ExprPtr>> fields;
    
    // Skip whitespace at start
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    if (!check(TokenType::RBRACE)) {
        do {
            // Skip whitespace before field
            while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
            std::string key = consume(TokenType::IDENTIFIER, "Expected field name").value;
            consume(TokenType::COLON, "Expected ':'");
            auto value = parseExpression();
            fields.push_back({key, value});
            // Skip whitespace after field
            while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
        } while (match(TokenType::COMMA));
    }
    
    // Skip whitespace before closing brace
    while (match(TokenType::NEWLINE) || match(TokenType::INDENT) || match(TokenType::DEDENT)) {}
    
    return fields;
}

}  // namespace construct
