#include "ast/VariableExprAST.h"

// 在Kaleidoscope设计的使用局部变量只在函数调用中，对于函数调用中的变量，一定已经在某处被定义了，所以直接查表，对于没有查到的报错，因为这不符合语言的设计
llvm::Value *VariableExprAST::Codegen() {
    llvm::Value *V = NamedValues[Name];
    return V ? V : LogErrorV("Unknown variable name");
}