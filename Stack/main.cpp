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
    stackInit           (&stk, 5    MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));
    stackPush           (&stk, 78   MAKE_DEBUG(argv[1]));

    //stackPush           ( NULL, 34  ON_DEBUG(, debug_info_t{ __FILE__, __func__, __LINE__ }));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    stackPop            (&stk       MAKE_DEBUG(argv[1]));
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
    changeMemoryUpStack (&stk, 100);
    printStackConsole   (&stk       MAKE_DEBUG(argv[1]));
}

/*
int stackPush(stack_t *stk, stack_element value)
{
    if (stk->size == stk->capacity)
    {
        stk->capacity *= 2;
        stk->data = realloc(stk->data, stk->capacity * sizeof(stack_element));
    }
    stk->data[stk->size++] = value;
}

stack_element stackPop(stack_t *stk)
{
    stack_element returnedData = stk->data[stk->size];
    if (stk->size == stk->capacity/4)
    {
        stk->capacity = (stk->capacity % 2 == 0 ? stk->capacity / 2 : (stk->capacity + 1) / 2);
        stk->data = realloc(stk->data, stk->capacity * sizeof(stack_element));
    }
    memset(stk->data, 0, sizeof(stk->data[stk->size]));
    stk->size--;
    return returnedData;
}
*/
