#include <iostream>
#include "is_the_server.h"
int main()
{

    if(IsServerIP("127.0.0.1",9000,4) < 0)
        printf("nonblocking connect error \n");

    return 0;
}
