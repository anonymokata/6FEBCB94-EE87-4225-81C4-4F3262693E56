#include <string.h>
#include "rpn.h"
typedef int bool;

#define RAII(type, name, size) \
    type name[size];           \
    memset(name, 0, size);

#define PUSH(var, index, value) \
    var[index++] = value;

#define PUSH_DEREF_INDEX(var, index, value) \
    var[*index] = value;                    \
    (*index)++;

#define POP_FROM_START(var, index) \
    var[index++]

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

void push_operator(char *operators, int *index, char op)
{
    int old_index = *index - 1;
    char old_op = operators[old_index];
    if (*index > 0 && op_less_than(op, operators[old_op]))
    {
        operators[old_index] = op;
        PUSH_DEREF_INDEX(operators, index, old_op);
    }
    else
    {
        PUSH_DEREF_INDEX(operators, index, op);
    }
}

char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen)
{
    if (buff == null || bufflen <= 0)
    {
        return null;
    }
    RAII(char, operands, 1 * 128);
    RAII(char, operators, 1 * 128);
    int opdIndexMax = 0, oprIndexMax = 0;
    for (int i = 0; i < infixmaxlen && infix[i] != '\0'; i++)
    {
        char inf = infix[i];
        if (valid_operand(inf))
        {
            PUSH(operands, opdIndexMax, inf);
        }
        else if (valid_operator(inf))
        {
            push_operator(operators, &oprIndexMax, inf);
        }
        else
            return null;
    }
    int buffIndex = 0;
    int opdIndex = 0;
    do
    {
        PUSH(buff, buffIndex, POP_FROM_START(operands, opdIndex));
    } while (opdIndex < opdIndexMax);
    int oprIndex = 0;
    do
    {
        PUSH(buff, buffIndex, POP_FROM_START(operators, oprIndex));
    } while (oprIndex < oprIndexMax);
    return buff;
}