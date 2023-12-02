#include "kaleidoscope.h"

llvm::LLVMContext TheContext;

llvm::IRBuilder<> Builder(TheContext);

std::unique_ptr<llvm::Module> TheModule;

std::map<std::string, llvm::Value *> NamedValues;