char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen, char *scratchbuff, int scratchbufflen);
char *rpn_to_infix(const char *rpn, int rpnmaxlen, char *buff, int bufflen, void *scratchbuff, int scratchbufflen, int forceParens);
