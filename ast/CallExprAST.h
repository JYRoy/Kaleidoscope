#ifndef __CALL_EXPR_AST_H__
#define __CALL_EXPR_AST_H__

#include "ast/ExprAST.h"
#include <memory>
#include <vector>


// 函数调用表达式
class CallExprAST: public ExprAST {
private:
    std::string Callee;  // 函数名
    std::vector<std::unique_ptr<ExprAST>> Args;  // 用作参数的表达式列表
public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {}
    virtual Value *Codegen();
};

#endif
