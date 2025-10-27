#ifndef CONSTRUCT_AST_PRINTER_H
#define CONSTRUCT_AST_PRINTER_H

#include "ast.h"
#include <iostream>
#include <string>
#include <memory>

namespace construct {

class ASTPrinter {
public:
    static void printProgram(const Program& program);
    static void printStatement(const StmtPtr& stmt, int indent = 0);
    static void printExpression(const ExprPtr& expr, int indent = 0);
    static void printType(const TypePtr& type);

private:
    static std::string indent(int level);
    static std::string typeToString(const TypePtr& type);
};

}  // namespace construct

#endif // CONSTRUCT_AST_PRINTER_H
