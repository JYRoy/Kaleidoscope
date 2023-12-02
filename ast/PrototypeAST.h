#ifndef __PROTOTYPE_AST_H__
#define __PROTOTYPE_AST_H__

#include "ast/ExprAST.h"
#include "llvm/IR/IRBuilder.h"
#include "kaleidoscope/kaleidoscope.h"

#include <string>
#include <vector>

// 函数接口原型
class PrototypeAST {
private:
    std::string Name;  // 函数名
    std::vector<std::string> Args;  // 函数参数列表
public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args): Name(name), Args(std::move(Args)) {}
    virtual llvm::Function *Codegen();
    const std::string &getName() const { return Name; }
};

#endif