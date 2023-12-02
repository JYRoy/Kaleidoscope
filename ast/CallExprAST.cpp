#include "ast/CallExprAST.h"

llvm::Value *CallExprAST::Codegen() {
  // TheModule记录函数, 因此从函数表中查找函数
  llvm::Function *CalleeF = TheModule->getFunction(Callee);

  if (!CalleeF) {
    return LogErrorV("Unknown function referenced");
  }

  if (CalleeF->arg_size() != Args.size()) {
    return LogErrorV("Incorrect # arguments passed");
  }
  // 生成传入的各个参数的代码
  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; i++) {
    ArgsV.push_back(Args[i]->Codegen());

    if (!ArgsV.back()) {
      return nullptr;
    }
  }
  // 生成call指令
  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}