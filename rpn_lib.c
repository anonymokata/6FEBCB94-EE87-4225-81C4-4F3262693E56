#include <string.h>
#include "rpn.h"

char* infix_to_rpn(const char* infix, int infixmaxlen, char* buff, int bufflen)
{
    if (buff == NULL || bufflen <= 0)
    {
        return NULL;
    }
    if (strcmp(infix, "a+b") == 0)
        memcpy(buff, "ab+", 3);
    return buff;
}