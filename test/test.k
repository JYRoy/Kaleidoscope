# Kaleidoscope支持函数、条件分支、数值计算等；

# 斐波那契数列
def fib(x)
    if x < 3 then
        1
    else
        fib(x - 1) + fib(x - 2)

fib(40)

# 函数声明
extern sin(arg)
extern cos(arg)
extern aten2(arg1 arg2)

# 声明后的函数可调用
aten(sin(.4), cos(42))