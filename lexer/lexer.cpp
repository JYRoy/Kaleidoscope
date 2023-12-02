#include "lexer/lexer.h"
#include "lexer/token.h"

int CurTok;  // 记录当前的token的ASCII码或者enum值
std::string IdentifierStr;  // 如果是字符，则用这个变量
double NumVal;  // 如果是字面值，则用这个变量（PS：设计的这个语言只支持float64）

// 从标准输入解析下一个token并返回
int gettok() {
    static int LastChar = ' ';

    while(isspace(LastChar)) {
        LastChar = getchar();
    }
    // 标识符
    if(isalpha(LastChar)) {
        IdentifierStr = LastChar;

        while(isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }

        if(IdentifierStr == "def") return token_def;

        if(IdentifierStr == "extern") return token_extern;

        return tok_identifier;
    }
    // [0-9.]+
    if(isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;

        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while(isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if(LastChar == '#') {
        // 遇到注释就跳转到行尾
        do {
            LastChar = getchar();
        } while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF) {
            return gettok();
        }
    }
    // 文件结束，返回结束符
    if(LastChar == EOF) {
        return token_eof;
    }

    // 返回ASCII值
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int getNextToken() {
    return CurTok = gettok();
}
