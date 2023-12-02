#include "ast/NumberExprAST.h"

// llvm中数值常量由ConstantFP类表示，具体数值由APFloat（Aribitrary Precision Float）表示
llvm::Value *NumberExprAST::Codegen() {
    return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
}