#include <stdlib.h>
#include <string.h>
#include "rpn.h"
typedef int bool;

#define MEMSET(buff, size) \
    memset(buff, 0, size);

#define RAII(type, name, size) \
    type name[size];           \
    MEMSET(name, size);

#define DRAII(type, name, size)               \
    type *name = malloc(sizeof(type) * size); \
    MEMSET(name, size);

#define DRAII_EXISTING(type, name, size) \
    name = malloc(sizeof(type) * size);  \
    MEMSET(name, size);

#define true 1
#define false 0
#ifndef null
#define null NULL
#endif
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
    if (op1 == '(')
        return true;
    byte op0val = find_op_value(op0);
    byte op1val = find_op_value(op1);
    return op0val - op1val < 0;
}

char peek(char *buff, int *index)
{
    return buff[*index - 1];
}

void push(char *buff, int *index, char c)
{
    buff[(*index)++] = c;
}

char pop(char *buff, int *index)
{
    if (*index > 0)
        return buff[--(*index)];
    return 0;
}

void swap(char *a, char *b)
{
    char c = *a;
    *a = *b;
    *b = c;
}

void handle_ops(char op, char *buff, int *buffindex, char *scratchbuff, int *scratchbufflen)
{
    if (*scratchbufflen > 0 && !op_less_than(op, peek(scratchbuff, scratchbufflen)))
    {
        push(scratchbuff, scratchbufflen, op);
        while (*scratchbufflen > 1 && scratchbuff[*scratchbufflen - 2] != '(' && op_less_than(scratchbuff[*scratchbufflen - 2], scratchbuff[*scratchbufflen - 1]))
        {
            swap(&scratchbuff[*scratchbufflen - 1], &scratchbuff[*scratchbufflen - 2]);
            push(buff, buffindex, pop(scratchbuff, scratchbufflen));
        }
    }
    else
    {
        push(scratchbuff, scratchbufflen, op);
    }
}

void unwind(char *buff, int *buffindex, char *scratchbuff, int *scratchbufflen)
{
    char op;
    do
    {
        op = pop(scratchbuff, scratchbufflen);
        if (op != '(')
        {
            push(buff, buffindex, op);
        }
    } while (op != '(' && *scratchbufflen > 0);
}

bool infix_to_rpn_impl(const char *infix, int infixmaxlen, int index, char *buff, int *buffindex, char *scratchbuff, int *scratchbufflen)
{
    if (infix[index] == '\0' || index >= infixmaxlen)
    {
        if (*scratchbufflen > 0)
        {
            unwind(buff, buffindex, scratchbuff, scratchbufflen);
        }
        return true;
    }
    char inf = infix[index];
    if (valid_operand(inf))
    {
        push(buff, buffindex, inf);
    }
    else if (valid_operator(inf))
    {
        handle_ops(inf, buff, buffindex, scratchbuff, scratchbufflen);
    }
    else if (valid_open_paren(inf))
    {
        push(scratchbuff, scratchbufflen, inf);
    }
    else if (valid_close_paren(inf))
    {
        unwind(buff, buffindex, scratchbuff, scratchbufflen);
    }
    bool ret = infix_to_rpn_impl(infix, infixmaxlen, index + 1, buff, buffindex, scratchbuff, scratchbufflen);
    if (!ret)
        return ret;
    return true;
}

char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen, char *scratchbuff, int scratchbufflen)
{
    if (infix == null || buff == null)
        return null;
    MEMSET(buff, bufflen);
    bool dynamic = scratchbuff == null || scratchbufflen == 0;
    if (dynamic)
    {
        scratchbufflen = infixmaxlen;
        DRAII_EXISTING(char, scratchbuff, scratchbufflen);
    }
    else
    {
        MEMSET(scratchbuff, scratchbufflen);
    }
    int sbl = 0;
    int bl = 0;
    bool ret = infix_to_rpn_impl(infix, infixmaxlen, 0, buff, &bl, scratchbuff, &sbl);
    if (dynamic)
    {
        free(scratchbuff);
    }
    return ret ? buff : null;
}

bool rpn_to_infix_impl(const char *rpn, int rpnmaxlen, int index, char *buff, int *buffindex, char *scratchbuff, int *scratchbufflen)
{
    if (rpn[index] == '\0' || index >= rpnmaxlen)
    {
        if (*scratchbufflen > 0)
        {
            push(buff, buffindex, ')');
        }
        return true;
    }
    char r = rpn[index];
    if (valid_operand(r))
    {
        if (index < rpnmaxlen && rpn[index + 1] != '\0' && valid_operand(rpn[index + 1]))
        {
            push(buff, buffindex, '(');
        }
        push(buff, buffindex, r);
    }
    else if (valid_operator(r))
    {
        push(buff, buffindex, r);
        swap(&buff[*buffindex - 1], &buff[*buffindex - 2]);
        push(buff, buffindex, ')');
    }
    bool ret = rpn_to_infix_impl(rpn, rpnmaxlen, index + 1, buff, buffindex, scratchbuff, scratchbufflen);
    if (!ret)
        return ret;
    return true;
}

char *rpn_to_infix(const char *rpn, int rpnmaxlen, char *buff, int bufflen, char *scratchbuff, int scratchbufflen)
{
    int sbl = 0;
    int bl = 0;
    bool ret = rpn_to_infix_impl(rpn, rpnmaxlen, 0, buff, &bl, scratchbuff, &sbl);

    return buff;
}