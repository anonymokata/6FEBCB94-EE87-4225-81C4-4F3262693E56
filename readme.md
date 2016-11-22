# specialized infix 2 rpn, rpn 2 infix converter

## Environment
- Ubuntu 14.04
- GCC 4.8.4
- Make 3.8.1
- Check 0.9.10
- Valgrind 3.10

```
sudo apt-get install build-essential check valgrind
```

Make and run tests:
```
make test
```

## API
```
#include "rpn.h"

char *infix_to_rpn(const char *infix, int infixmaxlen, char *buff, int bufflen, char *scratchbuff, int scratchbufflen);
infix: input string
infixmaxlen: maximum length of infix string. NOTE: null characters in infix are also checked
buff: output buffer
bufflen: output buffer length
scratchbuff: buffer used for temporary storage, leave blank for dynamic memory allocation
scratchbufflen: length of scratchbuff
returns: buff if success, null if not

char *rpn_to_infix(const char *rpn, int rpnmaxlen, char *buff, int bufflen, void *scratchbuff, int scratchbufflen, int forceParens);
rpn: input string
rpnmaxlen: maximum length of infix string. NOTE: null characters in infix are also checked
buff: output buffer
bufflen: output buffer length
scratchbuff: buffer used for temporary storage, leave blank for dynamic memory allocation
scratchbufflen: length of scratchbuff
forceParens: 1 to force output to have parentheses around each grouping, 0 for only required
returns: buff if success, null if not
```