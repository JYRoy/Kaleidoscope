#include "logger/logger.h"
#include "parser/parser.h"

std::map<char, int> BinopPrecedence;

// 查询二元运算符的优先级，如果不是二元运算符就返回-1
static int GetTokPrecedence() {
    if(!isascii(CurTok)) return -1;
    
    int TokPrec = BinopPrecedence[CurTok];
    if(TokPrec <= 0) return -1;
    return TokPrec;
}

// 解析数值常量
// numberexpr ::= number
std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);  // 构造NumberExprAST节点
    getNextToken();  // 使用词法分析器继续读取下一个token
    return std::move(Result);
}

// 解析括号运算符
// parenexpr ::= '(' expression ')'
// 调用该函数时，待解析的token一定是(，期望用户输入类似“(4)”，如果输入类似“(4x”就要报错
std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken();  // 消费(，获取下一个token

    auto V = ParseExpression();
    if (!V) {
        return nullptr;
    }

    if (CurTok != ')') {
       return LogError("Expected )");
    }

    getNextToken();  // 消费)，获取下一个token
    return V;
}


// 处理变量引用和函数调用
/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    // 预读(lookahead)：提前获取下一个token，据此判断当前标识符是变量（VariableExprAST）还是一个函数调用（CallExprAST）
    getNextToken();

    if (CurTok != '(')  // 如果不是函数(，说明不是一个函数调用
        return std::make_unique<VariableExprAST>(IdName);

    getNextToken();  // 是一个函数调用，如果有参数，下一个token就不是)
    std::vector<std::unique_ptr<ExprAST>> Args;  // 创建一个数组存储函数参数
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression()) {  // 函数的参数依然视作是表达式，例如：aten(sin(.4), cos(42))
                Args.push_back(std::move(Arg));
            } else {
                return nullptr;
            }

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                 return LogError("Expected ')' or ',' in argument list");

            getNextToken();
        }
    }

    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// 主表达式：判断待解析表达式的类别
/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
        default:
            return LogError("Unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();  // 如果是if表达式或者函数调用
        case tok_number:
            return ParseNumberExpr();      // primary是一个数字
        case '(':
            return ParseParenExpr();       // primary是以个(xxxxx)子表达式
    }
}
// expression 
//   ::= primary [binop primary] [binop primary] ... 
// a + b + (c + d) * e * f + g
//   ::= 首先解析​a​, 然后处理多个二元组​[+, b], [+, (c+d)], [*, e], [*, f], [+, g]
// 每个表达式都有一个主表达式开头，然后跟着一串有序对组成的列表，有序对格式为[binop, primaryexpr]
std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if(!LHS) return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

// 用于解析有序对列表
// RHS: Right Hand Side
// LHS: Left Hand Side，已经解析出来的表达式
// ExprPrec: min_precedence最小优先级
std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while(true) {
        int TokPrec = GetTokPrecedence();  // 检查当前token的优先级
        if(TokPrec < ExprPrec) return LHS;  // 如果优先级低就直接返回

        int BinOp = CurTok;  // 优先级满足条件，可以视为一个二元运算符，即 a + b 中的 +
        getNextToken();

        auto RHS = ParsePrimary();  // 获取紧跟其后的主表达式，即 a + b 中的 b
        if(!RHS) return nullptr;

        // 现在表达式的左侧和RHS序列中第一对都已经解析完毕，该考虑表达式的结合次序了。
        // 路有两条，要么选择“(a+b) binop unparsed”，要么选择“a + (b binop unparsed)”。
        // 为了搞清楚到底该怎么走，我们先预读出“binop”，查出它的优先级，再将之与BinOp（本例中是“+”）的优先级相比较：
        int NextPrec = GetTokPrecedence();
        if(TokPrec < NextPrec) {
            // 右边优先级更高，处理右边的操作符然后返回
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if(!RHS) return nullptr;
        }
        
        // a + b 被解析到一起
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// 解析函数原型
// 1. 函数声明
/// prototype
///   ::= id '(' id* ')'
std::unique_ptr<PrototypeAST> ParsePrototype() {
    if(CurTok != tok_identifier) return LogErrorP("Expected function name in prototype");  // def后的第一个token需要是一个函数名，函数名在该语言中被定义为tok_identifier

    std::string FnName = IdentifierStr;  // def后的第一个token是函数名
    getNextToken();  // 消费(

    if(CurTok != '(') return LogErrorP("Expected '(' in prototype");  // 判断是不是(

    std::vector<std::string> ArgNames;  // (之后是参数列表，创建也给数组存储参数列表
    while(getNextToken() == tok_identifier) {  // 迭代获取参数
        ArgNames.push_back(IdentifierStr);  // 每个参数应该都是一个字符串
    }
    if(CurTok != ')') return LogErrorP("Expected ')' in prototype");  // 参数列表结束后应该仅跟着)

    getNextToken();  // 消费)

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// 2. 函数定义
/// definition ::= 'def' prototype expression
std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken();  // 消费def
    auto Proto = ParsePrototype();
    if(!Proto) return nullptr;

    if(auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

std::unique_ptr<FunctionAST> ParseTopLevelExpr() { 
    if(auto expr = ParseExpression()) {
        auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>()); 
        return std::make_unique<FunctionAST>(std::move(proto), std::move(expr)); 
    }
    return nullptr;
}

// 解析tern语句，extern语句应被视为不带函数体的函数原型
/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken();  // 消费 extern.
    return ParsePrototype();  // 解析函数原型
}