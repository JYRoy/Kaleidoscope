#ifndef __BINARY_EXPR_AST_H__
#define __BINARY_EXPR_AST_H__

#include "llvm/IR/IRBuilder.h"
#include "logger/logger.h"
#include "ast/ExprAST.h"

#include <memory>

// 运算符
class BinaryExprAST: public ExprAST {
private:
    char Op;  // 保存运算符
    std::unique_ptr<ExprAST> LHS, RHS;  // 保存左值和右值
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    virtual llvm::Value *Codegen();
};

#endif