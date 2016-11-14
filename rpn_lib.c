#include <string.h>
#include "rpn.h"
typedef int bool;

#define RAII(type, name, size) \
    type name[size];           \
    memset(name, 0, size);

#define true 1
#define false 0
#define null NULL
#define byte unsigned char

char operator_lookup[] = {'^', '/', '*', '-', '+'};

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

bool valid_open_paren(char c)
{
    return c == '(';
}

bool valid_close_paren(char c)
{
    return c == ')';
}

byte find_op_value(char op)
{
    for (int i = 0; i < sizeof(operator_lookup); i++)
    {
        if (operator_lookup[i] == op)
            return i;
    }
    return -1;
}

bool op_less_than(char op0, char op1)
{
    byte op0val = find_op_value(op0);
    byte op1val = find_op_value(op1);
    return op0val - op1val < 0;
}

int opindex = 0;
char opbuff[128];//TODO: remove global
void push_op(char op)
{
    if (opindex > 0 && opbuff[opindex - 1] != '(' && !op_less_than(op, opbuff[opindex - 1]))
    {
        opbuff[opindex] = opbuff[opindex - 1];
        opbuff[opindex - 1] = op;
        opindex++;
    }
    else
    {
        opbuff[opindex++] = op;
    }
}

void infix_to_rpn_impl(const char *infix, int *index, char *buff, int *buffindex)
{
    char inf = infix[(*index)++];
    if (valid_operand(inf))
    {
        buff[(*buffindex)++] = inf;
    }
    else if (valid_operator(inf))
    {
        push_op(inf);
    }
    else if (valid_open_paren(inf))
    {
        opbuff[opindex++] = '(';
    }
    else if (valid_close_paren(inf))
    {
        do
        {
            if (opbuff[opindex - 1] == '(')
                break;
            buff[(*buffindex)++] = opbuff[opindex - 1];
        } while (opbuff[opindex--]);
    }
}

char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen)
{
    memset(opbuff, 0, sizeof(opbuff));
    int iindex = 0, buffindex = 0;
    while (infix[iindex] && iindex < infixmaxlen)
    {
        infix_to_rpn_impl(infix, &iindex, buff, &buffindex);
    }
    while (opindex > 0)
    {
        if (opbuff[opindex - 1] != '(')
            buff[buffindex++] = opbuff[opindex - 1];
        opindex--;
    }
    return buff;
}
