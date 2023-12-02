#ifndef __NUMBER_EXPR_AST_H__
#define __NUMBER_EXPR_AST_H__

#include "ast/ExprAST.h"

// 数值（字面值）表达式，ExprAST的子类
class NumberExprAST: public ExprAST {
private:
    double Val;  // 存放数值常量

public:
    NumberExprAST(double val): Val(val) {};
    virtual Value *Codegen();
};

#endif