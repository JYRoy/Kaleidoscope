#ifndef __LEXER_H__
#define __LEXER_H__

#include <cstdlib>
#include <string>

extern int CurTok;
int gettok();
int getNextToken();

extern std::string IdentifierStr;  // 如果当前token是标识符，用来保存标识符的名字
extern double NumVal;  // 如果当前token是数字，用来保存值，只支持float64类型；

#endif
