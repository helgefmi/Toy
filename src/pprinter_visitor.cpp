#include "pprinter_visitor.hpp"
#include <iostream>
#include <sstream>
#include "lexer.hpp"

void PrettyPrinterVisitor::visit(const AST*) {
}

void PrettyPrinterVisitor::visit(const ValueExpr*) {
}

void PrettyPrinterVisitor::visit(const BinaryOpExpr*) {
}

void PrettyPrinterVisitor::visit(const VariableExpr*) {
}

void PrettyPrinterVisitor::visit(const AssignExpr*) {
}

void PrettyPrinterVisitor::visit(const FuncCallExpr*) {
}

void PrettyPrinterVisitor::visit(const ExpressionStatement*) {
}

void PrettyPrinterVisitor::visit(const IfStatement*) {
}

void PrettyPrinterVisitor::visit(const WhileStatement*) {
}

void PrettyPrinterVisitor::visit(const ReturnStatement*) {
}

void PrettyPrinterVisitor::visit(const DefStatement*) {
}
