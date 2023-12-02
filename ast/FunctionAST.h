#ifndef __FUNCTION_AST_H__
#define __FUNCTION_AST_H__

#include "ast/PrototypeAST.h"
#include "kaleidoscope/kaleidoscope.h"

// 函数定义
class FunctionAST {
private:
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)), Body(std::move(Body)) {}
    virtual llvm::Function *Codegen();
};

#endif