#include "TXLib.h"
#include "stackWithCanary.cpp"


#define MAKE_DEBUG(arg) ON_DEBUG(, debug_info_t{ __FILE__, __func__, __LINE__, arg })
typedef int stack_element;



int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Write name for debug file\n");
        return 0;
    }
    stack_t stk = {};
    stackInit           (&stk, 1   MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    stackPop            (&stk       MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    changeMemoryUpStack (&stk, 100);
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    printf("Element = <%d>", stackTop(&stk MAKE_DEBUG(argv[1])));
}
