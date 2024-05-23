## 1.实验目的
本实验要求学生编写词法分析函数，从输入的源程序中，识别出各个具有独立意义的单词，即基本保留字、标识符、常数、运算符、分隔符五大类。并依次输出各个单词的内部编码及单词符号自身值。词法分析的目的是将输入的源程序进行划分，给出基本符号（token）的序列，并过滤注解和空格等分隔符，词法分析为下一个阶段的语法分析提供单词序列。
## 2.实验内容
### 1.词法分析目标语言
本次实验选择的目标语言是C语言。
C语言是计算机专业学生大一入学时用于入门程序设计的高级语言，同时也是许多理工科学生需要掌握的语言之一。其基本语法易于理解，方便阅读，相较于其他语言，比较容易进行词法分析。
### 2.词法分析实现语言
本次实验选用的实现项目的语言是C++。
C++ 拥有面向对象的特性，可以实现类的封装和方法设计，并且C++自带的STL功能强大，封装了各种常见的容器和基本数据结构，能够很大程度上满足我们的编程需求。同时我们的组员均较为熟练掌握C++，方便后续合作进行实验编码与测试。
实现词法分析器的以下功能：
1.给定正则表达式，将其转化为DAG
2.对源程序从左到右进行扫描，识别各个具有独立意义的词素，并将其转换成基本符号(token)输出
3.管理符号表，在分析结束后将符号表顺序输出
4.处理词法错误(要求进行错误定位，且能正常处理下一行)
5.剔除注释和空白符(空格、回车符、制表符)
实验指导：
1.设计有限状态自动机，参考教材83~84页；
2.设计缓冲区数组；
3.确定输入输出格式；
## 3. 词法单元和符号表设计
1. 标识符（Identifier）
1.自然语言描述：标识符是用来命名变量、函数、类等的名称，通常由字母、数字和下划线组
2.正规式描述：[a-zA-Z_][a-zA-Z0-9_]*
3.有穷自动机设计:
4.词法错误：
1.非法字符：标识符只能由字母、数字和下划线组成，如果包含其他字符（比如特殊符号），词法分析器会报告非法字符错误。
2.以数字开头：标识符不能以数字开头，如果一个标识符以数字开头，词法分析器会报告以数字开头的标识符错误。
2. 关键字（Keyword）
1.自然语言描述：关键字是编程语言中具有特殊含义的单词，不能被用作标识符。
2.正规式描述
auto	break	case	char	const
continue	else	default	do	double
else	enum	extern	float	for
goto	if	int	long	register
return	short	signed	sizeof	static
struct	switch	typedef	union	unsigned
void	volatile	while	true	
1.有穷自动机设计:同标识符，先判断是否为标识符，在判断是否为关键字
2.词法错误：不存在关键字表中
3. 数字（number)
1.自然语言描述：可以是整数、浮点数等。
2.正规式描述：digit+ (.digit+)?(E(+|–)?digit+)?
3.有穷自动机设计：
4.词法错误可能包括
1.非法字符：输入包含非数字、小数点、E、+、- 以外的字符。
2.小数点位置错误：小数点前后缺少数字或者多次出现小数点。
3.多个指数部分：只能有一个指数部分，如果出现多个 E，则属于词法错误。
4.小数点或指数部分缺失：小数点后面没有数字，或者缺少指数部分的数字。
4. 运算符（Operator）
1.自然语言描述：运算符用于进行各种运算操作，如加法、减法、乘法、除法等。
    算术运算符（+, -, *, /, %）
    关系运算符（==, !=, <=, >=, <, >）
    逻辑运算符（&&, ||, !）
    位运算符（&, |, ^, ~, <<, >>)
    赋值运算符（=, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=）
    其他运算符（?, :, ++, --, ->, .）
2.正规式描述：+\|-\|\|/\|%\|==\|!=\|<=\|>=\|<\|>\|&&\|||\|!\|&\||\|^\|<<\|>>\|=\|+=\|-=\|=\|/=\|%=\|&=\||=\|^=\|<<=\|>>=\|?\|:\|++\|–\|->\|.
3.有穷自动机设计：

4.词法错误

1.未定义的运算符：词法分析器只能识别预定义的运算符，如果输入中包含未定义的运算符，就会报告未知运算符错误。
5. 分隔符（Delimiter）
1.自然语言描述：分隔符用于分隔代码的不同部分，如括号、分号等。
2.正规式描述：( ) , [ ] ; { }  ” : ’ #
3.有穷自动机设计：

1.词法错误：
1.未定义的分隔符：词法分析器只能识别预定义的分隔符，如果输入中包含未定义的分隔符，就会报告未知分隔符错误。
## 4.算法说明
1. 正则表达式的预处理
1.预处理是把表达式中省略的连接运算符加上，为当第一位是操作数、“*”，“）”，“+”，“？”且第二位为操作数或“（”，在他们之间添加“&”作为连接符
2.中缀表达式转后缀表达式
1.运算符的优先级为 ’*‘,’+‘,’?’ >‘&’ > ‘|’> ‘(’
2.如果遇到操作数，直接将其输出。
3.如果遇到运算符：
1.遇到‘（’直接压入栈中；
2.遇到‘）’将运算符出栈并输出，直到遇到‘（’，将‘（’出栈但不输出；
3.遇到‘*’、‘&’、‘|’、‘+’、’?’运算符：
1.如果栈为空，直接将运算符压入栈中；
2.如果栈不为空，弹出栈中优先级大于等于当前运算符的运算符并输出，再将当前运算符入栈。
4.当输入串读取完之后，如果栈不为空，则将栈中元素依次出栈并输出。
3.由于正则表达式中可能有”+“，“”,所以闭包运算符“”等特殊符号前面要加上’',变成’*’。
2. 后缀表达式创建NFA
1.按顺序读取后缀表达式，每次读取一个字符
1.如果遇到操作数a，则新建一个NFA，转换弧上的值为a，将这个NFA压入栈中


2.如果遇到闭包运算符“*”，则新建一个NFA n，从NFA栈中弹出一个元素 n1，连接关系如下，将NFA n压入栈中



3.如果遇到或运算符“|”，则新建一个NFA n，并在NFA栈中弹出两个元素n1，n2，连接关系如下，将NFA n压入栈中



4.如果遇到连接运算符“&”，不用新建NFA，只需在NFA栈中弹出两个元素n1，n2，改变n1 n2的头尾指针，连接关系如下，最后将n压入栈中

5.如果遇到闭包运算符“+”，则新建一个NFA n，从NFA栈中弹出一个元素 n1，连接关系如下，将NFA n压入栈中


6.如果遇到闭包运算符“？”，则NFA栈中弹出一个元素 n，将n的head向tail连边，将NFA n压入栈中
3. NFA转化为DFA
1.ε-closure(T)的计算
2.
3.
由NFA构造DFA的子集构造算法
4.

4. 判断字符串类型
1.当输入的多个前缀与多个模式匹配时，使用如下优先原则选择正确的词素：
1.匹配最长串的规则优先
2.长度相同时，在前的规则优先
2.DFA.find():在该DFA上能检测这个字符串的最长长度

3.DFA.cal():
