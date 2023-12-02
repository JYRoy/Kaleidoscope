#ifndef __VARIABLE_EXPR_AST_H__
#define __VARIABLE_EXPR_AST_H__

#include "ast/ExprAST.h"


// 保存变量
class VariableExprAST: public ExprAST {
private:
    std::string Name;  // 存放变量名

public:
    VariableExprAST(const std::string &name): Name(name) {}
    virtual Value *Codegen();
};

#endif