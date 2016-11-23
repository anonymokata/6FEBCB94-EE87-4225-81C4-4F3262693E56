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

#define RET_BUFF_OVERFLOW(buffindex, bufflen) \
    if (*buffindex > bufflen)                 \
        return false;

#define true 1
#define false 0
#ifndef null
#define null NULL
#endif
#define byte unsigned char

typedef struct node
{
    struct node *lhs, *rhs;
    char c;
} node;

bool walk_side(node *root, node *side, char *buff, int *buffindex, int bufflen, bool forceParens);
bool walk(node *root, char *buff, int *buffindex, int bufflen, bool forceParens);

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
    for (unsigned long i = 0; i < sizeof(operator_lookup); i++)
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

void handle_ops(char op, char *buff, int *buffindex, char *tempbuff, int *tempbufflen)
{
    if (*tempbufflen > 0 && !op_less_than(op, peek(tempbuff, tempbufflen)))
    {
        PUSH(tempbuff, tempbufflen, op);
        while (*tempbufflen > 1 && tempbuff[*tempbufflen - 2] != '(' && op_less_than(tempbuff[*tempbufflen - 2], tempbuff[*tempbufflen - 1]))
        {
            swap(&tempbuff[*tempbufflen - 1], &tempbuff[*tempbufflen - 2]);
            PUSH(buff, buffindex, POP(tempbuff, tempbufflen));
        }
    }
    else
    {
        PUSH(tempbuff, tempbufflen, op);
    }
}

void unwind(char *buff, int *buffindex, char *tempbuff, int *tempbufflen)
{
    char op;
    do
    {
        op = POP(tempbuff, tempbufflen);
        if (op != '(')
        {
            PUSH(buff, buffindex, op);
        }
    } while (op != '(' && *tempbufflen > 0);
}

bool infix_to_rpn_impl(const char *infix, int infixmaxlen, int index, char *buff, int *buffindex, char *tempbuff, int *tempbufflen)
{
    if (infix[index] == '\0' || index >= infixmaxlen)
    {
        if (*tempbufflen > 0)
        {
            unwind(buff, buffindex, tempbuff, tempbufflen);
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
        handle_ops(inf, buff, buffindex, tempbuff, tempbufflen);
    }
    else if (valid_open_paren(inf))
    {
        PUSH(tempbuff, tempbufflen, inf);
    }
    else if (valid_close_paren(inf))
    {
        unwind(buff, buffindex, tempbuff, tempbufflen);
    }
    bool ret = infix_to_rpn_impl(infix, infixmaxlen, index + 1, buff, buffindex, tempbuff, tempbufflen);
    if (!ret)
        return ret;
    return true;
}

char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen, char *tempbuff, int tempbufflen)
{
    if (infix == null || buff == null)
        return null;
    MEMSET(buff, bufflen);
    bool dynamic = tempbuff == null || tempbufflen == 0;
    if (dynamic)
    {
        tempbufflen = infixmaxlen;
        DRAII_EXISTING(char, tempbuff, tempbufflen);
    }
    else
    {
        MEMSET(tempbuff, tempbufflen);
    }
    int sbl = 0;
    int bl = 0;
    bool ret = infix_to_rpn_impl(infix, infixmaxlen, 0, buff, &bl, tempbuff, &sbl);
    if (dynamic)
    {
        free(tempbuff);
    }
    return ret ? buff : null;
}

bool walk_side(node *root, node *side, char *buff, int *buffindex, int bufflen, bool forceParens)
{
    bool paren = false;
    if (valid_operator(root->c) && valid_operator(side->c) && (op_less_than(root->c, side->c) || forceParens))
    {
        paren = true;
        RET_BUFF_OVERFLOW(buffindex, bufflen);
        PUSH(buff, buffindex, '(');
    }
    bool ret = walk(side, buff, buffindex, bufflen, forceParens);
    if (!ret)
        return ret;
    if (paren)
    {
        RET_BUFF_OVERFLOW(buffindex, bufflen);
        PUSH(buff, buffindex, ')');
    }
    return true;
}

bool walk(node *root, char *buff, int *buffindex, int bufflen, bool forceParens)
{
    if (root->lhs != null)
    {
        bool ret = walk_side(root, root->lhs, buff, buffindex, bufflen, forceParens);
        if (!ret)
            return ret;
    }
    RET_BUFF_OVERFLOW(buffindex, bufflen);
    PUSH(buff, buffindex, root->c);
    if (root->rhs != null)
    {
        bool ret = walk_side(root, root->rhs, buff, buffindex, bufflen, forceParens);
        if (!ret)
            return ret;
    }
    free(root);
    return true;
}

node **rpn_to_infix_impl(const char *rpn, int rpnmaxlen, void *tempbuff, int tempbufflen)
{
    node **stack = tempbuff;
    int buffindex = 0;
    for (int i = 0; i < rpnmaxlen && rpn[i] != '\0'; i++)
    {
        char c = rpn[i];
        node *n = NEW(node, n);
        n->c = c;
        if (valid_operand(c))
        {
            if (buffindex > tempbufflen)
                return null;
            PUSH(stack, &buffindex, n);
        }
        else if (valid_operator(c))
        {
            node *rhs = POP(stack, &buffindex);
            node *lhs = POP(stack, &buffindex);
            n->rhs = rhs;
            n->lhs = lhs;
            if (buffindex > tempbufflen)
                return null;
            PUSH(stack, &buffindex, n);
        }
    }
    return stack;
}

char *rpn_to_infix(const char *rpn, int rpnmaxlen, char *buff, int bufflen, void *tempbuff, int tempbufflen, int forceParens)
{
    bool dynamic = tempbuff == null || tempbufflen == 0;
    if (dynamic)
    {
        tempbufflen = rpnmaxlen;
        DRAII_EXISTING(char, tempbuff, tempbufflen);
    }
    else
    {
        MEMSET(tempbuff, tempbufflen);
    }
    node **stack = rpn_to_infix_impl(rpn, rpnmaxlen, tempbuff, tempbufflen);
    if (dynamic)
    {
        free(tempbuff);
    }
    if (stack != null)
    {
        int buffindex = 0;
        bool success = walk(stack[0], buff, &buffindex, bufflen, forceParens);
        if (success)
        {
            return buff;
        }
    }
    return null;
}
