#include "ast_printer.h"
#include <sstream>
#include <iomanip>

namespace construct {

std::string ASTPrinter::indent(int level) {
    return std::string(level * 2, ' ');
}

std::string ASTPrinter::typeToString(const TypePtr& type) {
    if (!type) return "?";
    
    switch (type->kind) {
        case Type::INT:
            return "Int";
        case Type::FLOAT:
            return "Float";
        case Type::BOOL:
            return "Bool";
        case Type::STRING:
            return "String";
        case Type::LIST:
            return "[" + typeToString(type->element_type) + "]";
        case Type::FUNCTION:
            return typeToString(type->param_type) + " -> " + typeToString(type->return_type);
        case Type::TUPLE:
            {
                std::string result = "(";
                for (size_t i = 0; i < type->element_types.size(); ++i) {
                    if (i > 0) result += ", ";
                    result += typeToString(type->element_types[i]);
                }
                result += ")";
                return result;
            }
        case Type::VARIABLE:
            return "'" + type->var_name;
        case Type::UNKNOWN:
            return "?";
        default:
            return "Unknown";
    }
}

void ASTPrinter::printProgram(const Program& program) {
    std::cout << "=== AST ===" << std::endl;
    if (program.statements.empty()) {
        std::cout << "(empty program)" << std::endl;
        return;
    }
    
    for (const auto& stmt : program.statements) {
        printStatement(stmt, 0);
    }
    std::cout << "=== End AST ===" << std::endl;
}

void ASTPrinter::printStatement(const StmtPtr& stmt, int ind) {
    if (!stmt) return;
    
    switch (stmt->kind) {
        case Statement::LET_BINDING:
            std::cout << indent(ind) << "LET " << stmt->name;
            if (stmt->type_annotation) {
                std::cout << ": " << typeToString(stmt->type_annotation);
            }
            std::cout << " =" << std::endl;
            printExpression(stmt->expr, ind + 1);
            break;
            
        case Statement::FUNCTION_DEF:
            std::cout << indent(ind) << "FN " << stmt->name;
            if (stmt->expr && stmt->expr->kind == Expr::LAMBDA) {
                std::cout << "(";
                const auto& params = stmt->expr->parameters;
                for (size_t i = 0; i < params.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << params[i].name;
                    if (params[i].type) {
                        std::cout << ": " << typeToString(params[i].type);
                    }
                }
                std::cout << ")";
                if (stmt->expr->return_type) {
                    std::cout << " -> " << typeToString(stmt->expr->return_type);
                }
            }
            std::cout << std::endl;
            if (stmt->expr) {
                printExpression(stmt->expr, ind + 1);
            }
            break;
            
        case Statement::TYPE_DEF:
            std::cout << indent(ind) << "TYPE " << stmt->name << " = " 
                      << typeToString(stmt->type_value) << std::endl;
            break;
            
        case Statement::IMPORT:
            std::cout << indent(ind) << "IMPORT " << stmt->module_name << std::endl;
            break;
            
        case Statement::INCLUDE:
            std::cout << indent(ind) << "INCLUDE";
            for (const auto& path : stmt->include_paths) {
                std::cout << " " << path;
            }
            std::cout << std::endl;
            break;
            
        case Statement::EXPR_STMT:
            std::cout << indent(ind) << "EXPR" << std::endl;
            printExpression(stmt->expr, ind + 1);
            break;
    }
}

void ASTPrinter::printExpression(const ExprPtr& expr, int ind) {
    if (!expr) return;
    
    switch (expr->kind) {
        case Expr::INT_LITERAL:
            std::cout << indent(ind) << "INT(" << expr->literal_value << ")" << std::endl;
            break;
            
        case Expr::FLOAT_LITERAL:
            std::cout << indent(ind) << "FLOAT(" << expr->literal_value << ")" << std::endl;
            break;
            
        case Expr::STRING_LITERAL:
            std::cout << indent(ind) << "STRING(\"" << expr->literal_value << "\")" << std::endl;
            break;
            
        case Expr::BOOL_LITERAL:
            std::cout << indent(ind) << "BOOL(" << expr->literal_value << ")" << std::endl;
            break;
            
        case Expr::IDENTIFIER:
            std::cout << indent(ind) << "ID(" << expr->name << ")" << std::endl;
            break;
            
        case Expr::BINARY_OP:
            std::cout << indent(ind) << "BINOP(" << expr->op << ")" << std::endl;
            std::cout << indent(ind) << "  left:" << std::endl;
            printExpression(expr->left, ind + 2);
            std::cout << indent(ind) << "  right:" << std::endl;
            printExpression(expr->right, ind + 2);
            break;
            
        case Expr::UNARY_OP:
            std::cout << indent(ind) << "UNOP(" << expr->op << ")" << std::endl;
            printExpression(expr->operand, ind + 1);
            break;
            
        case Expr::APPLICATION:
            std::cout << indent(ind) << "CALL" << std::endl;
            std::cout << indent(ind) << "  function:" << std::endl;
            printExpression(expr->function_expr, ind + 2);
            std::cout << indent(ind) << "  args:" << std::endl;
            for (const auto& arg : expr->arguments) {
                printExpression(arg, ind + 2);
            }
            break;
            
        case Expr::IF_EXPR:
            std::cout << indent(ind) << "IF" << std::endl;
            std::cout << indent(ind) << "  condition:" << std::endl;
            printExpression(expr->condition, ind + 2);
            std::cout << indent(ind) << "  then:" << std::endl;
            printExpression(expr->then_expr, ind + 2);
            std::cout << indent(ind) << "  else:" << std::endl;
            printExpression(expr->else_expr, ind + 2);
            break;
            
        case Expr::MATCH_EXPR:
            std::cout << indent(ind) << "MATCH" << std::endl;
            std::cout << indent(ind) << "  value:" << std::endl;
            printExpression(expr->match_value, ind + 2);
            std::cout << indent(ind) << "  cases:" << std::endl;
            for (const auto& case_item : expr->cases) {
                std::cout << indent(ind + 2) << "case:" << std::endl;
                std::cout << indent(ind + 3) << "pattern: (not implemented)" << std::endl;
                printExpression(case_item.body, ind + 3);
            }
            break;
            
        case Expr::LAMBDA:
            std::cout << indent(ind) << "FN" << std::endl;
            if (!expr->parameters.empty()) {
                std::cout << indent(ind) << "  params:" << std::endl;
                for (const auto& param : expr->parameters) {
                    std::cout << indent(ind + 2) << param.name;
                    if (param.type) {
                        std::cout << ": " << typeToString(param.type);
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << indent(ind) << "  body:" << std::endl;
            printExpression(expr->body, ind + 2);
            break;
            
        case Expr::BLOCK_FN:
            std::cout << indent(ind) << "BLOCK_FN(" << expr->fn_name << ")" << std::endl;
            std::cout << indent(ind) << "  body:" << std::endl;
            printExpression(expr->body, ind + 2);
            break;
            
        case Expr::LIST_LITERAL:
            std::cout << indent(ind) << "LIST" << std::endl;
            for (const auto& elem : expr->elements) {
                printExpression(elem, ind + 1);
            }
            break;
            
        case Expr::LIST_COMP:
            std::cout << indent(ind) << "LIST_COMP" << std::endl;
            std::cout << indent(ind) << "  expr:" << std::endl;
            printExpression(expr->comp_expr, ind + 2);
            std::cout << indent(ind) << "  bindings:" << std::endl;
            for (const auto& binding : expr->comp_bindings) {
                std::cout << indent(ind + 2) << binding.var_name << " in" << std::endl;
                printExpression(binding.iterable, ind + 3);
            }
            break;
            
        case Expr::RECORD_LITERAL:
            std::cout << indent(ind) << "RECORD" << std::endl;
            for (const auto& [name, val] : expr->record_fields) {
                std::cout << indent(ind + 1) << name << ":" << std::endl;
                printExpression(val, ind + 2);
            }
            break;
            
        case Expr::TUPLE_LITERAL:
            std::cout << indent(ind) << "TUPLE" << std::endl;
            for (const auto& elem : expr->elements) {
                printExpression(elem, ind + 1);
            }
            break;
            
        case Expr::FIELD_ACCESS:
            std::cout << indent(ind) << "FIELD_ACCESS(" << expr->field_name << ")" << std::endl;
            std::cout << indent(ind) << "  record:" << std::endl;
            printExpression(expr->record_expr, ind + 2);
            break;
            
        case Expr::RECORD_UPDATE:
            std::cout << indent(ind) << "RECORD_UPDATE" << std::endl;
            std::cout << indent(ind) << "  base:" << std::endl;
            printExpression(expr->base_record, ind + 2);
            std::cout << indent(ind) << "  updates:" << std::endl;
            for (const auto& [name, val] : expr->update_fields) {
                std::cout << indent(ind + 2) << name << ":" << std::endl;
                printExpression(val, ind + 3);
            }
            break;
            
        case Expr::PIPE:
            std::cout << indent(ind) << "PIPE" << std::endl;
            std::cout << indent(ind) << "  left:" << std::endl;
            printExpression(expr->left, ind + 2);
            std::cout << indent(ind) << "  right:" << std::endl;
            printExpression(expr->right, ind + 2);
            break;
            
        case Expr::FOR_LOOP:
            std::cout << indent(ind) << "FOR " << expr->loop_var << " in" << std::endl;
            printExpression(expr->iterable, ind + 1);
            std::cout << indent(ind) << "  do:" << std::endl;
            printExpression(expr->loop_body, ind + 2);
            break;
            
        case Expr::WHILE_LOOP:
            std::cout << indent(ind) << "WHILE" << std::endl;
            std::cout << indent(ind) << "  condition:" << std::endl;
            printExpression(expr->condition, ind + 2);
            std::cout << indent(ind) << "  do:" << std::endl;
            printExpression(expr->loop_body, ind + 2);
            break;
            
        case Expr::BLOCK:
            std::cout << indent(ind) << "BLOCK" << std::endl;
            for (const auto& stmt : expr->elements) {
                printExpression(stmt, ind + 1);
            }
            break;
    }
}

}  // namespace construct
