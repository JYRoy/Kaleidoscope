#include "ast/FunctionAST.h"

llvm::Function *FunctionAST::Codegen() {
  NamedValues.clear();  // 清理之前残留的参数
  // 获取与函数名相对应的函数对象
  llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

  if (!TheFunction) {
    TheFunction = Proto->Codegen();  // 生成函数原型的代码
  }

  if (!TheFunction) {
    return nullptr;
  }
  // 生成基本块, 这是用于定义控制流图的重要部件, 但是因为当前的Kaleidoscope不涉及控制流, 所以所有函数只有一个基本快。
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", TheFunction);
  Builder.SetInsertPoint(BB);  // 告诉Builder新指令应该插至刚刚新建的基本快的末尾
  

  // 并参数注册倒NamedValues映射表内
  for (auto &Arg : TheFunction->args()) {
    NamedValues[Arg.getName().data()] = &Arg;
  }

    // 设置好插入带你后，调用函数主表达式的CodeGen方法
  if (llvm::Value *RetVal = Body->Codegen()) {
    Builder.CreateRet(RetVal);  // 再基本块中创建一个返回RetVal类型的指令
    verifyFunction(*TheFunction);

    return TheFunction;
  }

  TheFunction->eraseFromParent();
  return nullptr;
}