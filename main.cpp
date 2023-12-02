// lexer headers
#include "lexer/lexer.h"
#include "lexer/token.h"

// AST headers
#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"

// parser headers
#include "parser/parser.h"

// logger headers
#include "logger/logger.h"

// stdlib headers
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>

static void HandleDefinition() {
    if(auto FnAST = ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken();
    }
}

static void HandleExtern() {
    if(ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if(ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        getNextToken();
    }
}

static void MainLoop() {
    while(true) {
        fprintf(stderr, "ready> ");
        switch(CurTok) {
            case token_eof:                       // 遇到结束符，程序退出
                return;
            case ';':                             // 遇到分号，当前表达式结束，继续读取下一个字符
                getNextToken();
                break;
            case token_def:                       // 遇到def关键字，分析def语句
                HandleDefinition();
                break;
            case token_extern:                    // 遇到extern关键字，分析extern关键字
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();       // 解析表达式
                break;
        }
    }
}

int main() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
    fprintf(stderr, "ready> ");
    getNextToken();
    MainLoop();
    return 0;
}