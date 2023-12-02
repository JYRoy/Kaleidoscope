#include "ast/PrototypeAST.h"

llvm::Function *PrototypeAST::Codegen() {
  // 因为Kaleidoscope只支持double类型,所以创建一个N个double参数的vector
  std::vector<llvm::Type *> Doubles(Args.size(), llvm::Type::getDoubleTy(TheContext));

  // 用于为给定的函数原型创建对应的FunctionType对象
  // static FunctionType * 	get (Type *Result, ArrayRef< Type * > Params, bool isVarArg)
  //	This static method is the primary way of constructing a FunctionType.

  // 单个double作为输出, N个double的vector作为输入,  false表示参数个数不可变
  // llvm中的类型对象也是单例, 即通过get而不是new获取
  llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(TheContext), Doubles, false);

  // 创建的是与该函数原型相对应的函数, 包含了函数名, 类型和链接方式等信息
  // llvm::Function::ExternalLinkage: 表示该函数可能定义于当前模块之外,且/或可以被当前模块之外的函数调用
  llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

  // 遍历函数原型的所有参数，为这些LLVM Argument对象逐一设置参数名
  unsigned Idx = 0;
  for (auto &Arg : F->args()) {
    Arg.setName(Args[Idx++]);
  }

  return F;
}