#include "ast/BinaryExprAST.h"
#include "kaleidoscope/kaleidoscope.h"

// 递归生成代码，先处理表达式的左侧，再处理表达式的右侧，最后计算整个二元表达式的值
llvm::Value *BinaryExprAST::Codegen() {
  llvm::Value *L = LHS->Codegen();
  llvm::Value *R = RHS->Codegen();

  if (!L || !R) {
    return nullptr;
  }

  switch (Op) {
    case '+':
      // CreateFADD：计算指令
      // L, R: 操作数
      // "addtmp": 生成的指令的名字
      // LLVM指令严格遵守的约束: 例如add指令的L,R操作数必须是同一个类型, 结果的类型必须与操作数的类型相容.
      return Builder.CreateFAdd(L, R, "addtmp");
    case '-':
      return Builder.CreateFSub(L, R, "subtmp");
    case '*':
      return Builder.CreateFMul(L, R, "multmp");
    case '<':
      // LLVM要求fcmp指令的返回值类型必须是‘i1’（单比特整数）
      L = Builder.CreateFCmpULT(L, R, "cmptmp");
      // uitofp指令会将输入的整数视作无符号数，并将之转换成浮点数
      return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(TheContext), "booltmp");
    default:
      return LogErrorV("Invalid binary operator");
  }
}