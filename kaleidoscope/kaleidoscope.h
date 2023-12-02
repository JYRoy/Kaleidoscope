#ifndef __KALEIDOSCOPE_H__
#define __KALEIDOSCOPE_H__

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <map>

extern llvm::LLVMContext TheContext;

// 用于简化LLVM指令生成的辅助对象。IRBuilder用于跟踪当前插入指令的位置，及带有用于生成新指令的方法
extern llvm::IRBuilder<> Builder;

// 用于记录函数及全局变量
extern std::unique_ptr<llvm::Module> TheModule;

// 用于记录当前作用域内的变量及对应的LLVM表示
extern std::map<std::string, llvm::Value *> NamedValues;

#endif