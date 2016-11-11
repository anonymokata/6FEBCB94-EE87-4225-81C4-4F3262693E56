#include <string.h>
#include "rpn.h"
typedef int bool;

#define RAII(type, name, size) \
    type name[size];\
    memset(name, 0, size);

#define PUSH(var, index, value) \
    var[index++] = value;

#define POP_FROM_START(var, index) \
    var[index++]

#define true 1
#define false 0
#define null NULL

bool valid_operand(char c)
{
    if (c >= 'a' && c <= 'z')
        return true;
    return false;
}

bool valid_operator(char c)
{
    return c == '^' || c == '/' || c == '*' || c == '-' || c == '+';
}

char* infix_to_rpn(const char* infix, int infixmaxlen, char* buff, int bufflen)
{
    if (buff == null || bufflen <= 0)
    {
        return null;
    }
    RAII(char, operands, 1 * 1024);
    RAII(char, operators, 1 * 1024);
    int opdIndexMax = 0, oprIndexMax = 0;
    for(int i = 0; i < infixmaxlen && infix[i] != '\0'; i++)
    {
        char inf = infix[i];
        if (valid_operand(inf))
        {
            PUSH(operands, opdIndexMax, inf);
        }
        else if (valid_operator(inf))
        {
            PUSH(operators, oprIndexMax, inf);
        }
        else
            return null;
    }
    int buffIndex = 0;
    int opdIndex = 0;
    do
    {
        PUSH(buff, buffIndex, POP_FROM_START(operands, opdIndex));
    } while(opdIndex < opdIndexMax);
    int oprIndex = 0;
    do
    {
        PUSH(buff, buffIndex, POP_FROM_START(operators, oprIndex));
    } while(oprIndex < oprIndexMax);
    return buff;
}