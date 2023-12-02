#ifndef __TOKEN_H__
#define __TOKEN_H__

// 定义5种内置标识符，其他的返回[0-255]的ASCII值
enum Token {
    token_eof = -1,
    token_def = -2,
    token_extern = -3,
    tok_identifier = -4,
    tok_number = -5
};

#endif
