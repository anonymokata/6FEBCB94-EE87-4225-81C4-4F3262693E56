char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen, char *tempbuff, int tempbufflen);
char *rpn_to_infix(const char *rpn, int rpnmaxlen, char *buff, int bufflen, void *tempbuff, int tempbufflen, int forceParens);
