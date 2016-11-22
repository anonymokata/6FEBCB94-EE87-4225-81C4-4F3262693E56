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

#define NEW(type, name)   \
    malloc(sizeof(type)); \
    MEMSET(name, sizeof(type));

#define PUSH(name, index, val) \
    name[(*index)++] = val;

#define POP(buff, index) \
    buff[--(*index)];

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
        PUSH(scratchbuff, scratchbufflen, op);
        while (*scratchbufflen > 1 && scratchbuff[*scratchbufflen - 2] != '(' && op_less_than(scratchbuff[*scratchbufflen - 2], scratchbuff[*scratchbufflen - 1]))
        {
            swap(&scratchbuff[*scratchbufflen - 1], &scratchbuff[*scratchbufflen - 2]);
            PUSH(buff, buffindex, POP(scratchbuff, scratchbufflen));
        }
    }
    else
    {
        PUSH(scratchbuff, scratchbufflen, op);
    }
}

void unwind(char *buff, int *buffindex, char *scratchbuff, int *scratchbufflen)
{
    char op;
    do
    {
        op = POP(scratchbuff, scratchbufflen);
        if (op != '(')
        {
            PUSH(buff, buffindex, op);
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
        PUSH(buff, buffindex, inf);
    }
    else if (valid_operator(inf))
    {
        handle_ops(inf, buff, buffindex, scratchbuff, scratchbufflen);
    }
    else if (valid_open_paren(inf))
    {
        PUSH(scratchbuff, scratchbufflen, inf);
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

typedef struct node
{
    struct node *lhs, *rhs;
    char c;
} node;

bool walk(node *root, char *buff, int *bufflen)
{
    if (root->lhs != null)
    {
        bool paren = false;
        if (valid_operator(root->c) && valid_operator(root->lhs->c) && op_less_than(root->c, root->lhs->c))
        {
            paren = true;
            PUSH(buff, bufflen, '(');
        }
        walk(root->lhs, buff, bufflen);
        if (paren)
        {
            PUSH(buff, bufflen, ')');
        }
    }
    PUSH(buff, bufflen, root->c);
    if (root->rhs != null)
    {
        bool paren = false;
        if (valid_operator(root->c) && valid_operator(root->rhs->c) && op_less_than(root->c, root->rhs->c))
        {
            paren = true;
            PUSH(buff, bufflen, '(');
        }
        walk(root->rhs, buff, bufflen);
        if (paren)
        {
            PUSH(buff, bufflen, ')');
        }
    }
    free(root);
    return true;
}

node **rpn_to_infix_impl(const char *rpn, int rpnmaxlen, void *scratchbuff, int scratchbufflen)
{
    node **stack = scratchbuff;
    int buffindex = 0;
    for (int i = 0; i < rpnmaxlen && rpn[i] != '\0'; i++)
    {
        char c = rpn[i];
        node *n = NEW(node, n);
        n->c = c;
        if (valid_operand(c))
        {
            PUSH(stack, &buffindex, n);
        }
        else if (valid_operator(c))
        {
            node *rhs = POP(stack, &buffindex);
            node *lhs = POP(stack, &buffindex);
            n->rhs = rhs;
            n->lhs = lhs;
            PUSH(stack, &buffindex, n);
        }
    }
    return stack;
}

char *rpn_to_infix(const char *rpn, int rpnmaxlen, char *buff, int bufflen, void *scratchbuff, int scratchbufflen)
{
    int sbl = 0;
    int bl = 0;
    node **stack = rpn_to_infix_impl(rpn, rpnmaxlen, scratchbuff, scratchbufflen);
    if (stack != null)
    {
        int buffindex = 0;
        walk(stack[0], buff, &buffindex);
    }
    return buff;
}