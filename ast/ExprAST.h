#ifndef __EXPR_AST_H__
#define __EXPR_AST_H__

// 表达式节点的基类
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value *Codegen() = 0;
};

#endif