#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stackHeader.h"




#ifdef STK_DEBUG
void makeDebugLog(const stack_t *const stk, debug_info_t debugInfo, errors errCode)
{
    FILE *file = nullptr;
    if((file = fopen(debugInfo.fileName, "ab")) == NULL)
    {
        printf("Cannot open file for debug\n");\
        assert(NULL);
        return;
    }

    if (stk == NULL)
    {

        fprintf(file, "Stack wasn't initialised. Created by %s() at %s:%d\n Error code: %d - %s\n",
                debugInfo.func, debugInfo.file, debugInfo.line, errCode, getErrorName(errCode));
        fclose(file);
        return;
    }

    fprintf(file, "created by %s() at %s:%d\n Error code: %d - %s\n Size = <%zu>\n Capacity = <%zu>\n data[%p]:\n",
            debugInfo.func, debugInfo.file, debugInfo.line, errCode, getErrorName(errCode), stk->size, stk->capacity, (void*)stk->data);

    if(stk->data != 0)
    {
        fprintf(file, "{\n");

        for (size_t i = 0; i < stk->capacity; i++)
        {
            if(i < stk->size)
                fprintf(file, "* ");

            fprintf(file, "[%zu] = <%d>\n", i, stk->data[i]);
        }

        fprintf(file, "}\n");
    }

    fprintf(file, "\n\n\n\n\n");
    fclose(file);
}
#endif






void canaryAdder(stack_element_t *canaryData, size_t capacity)
{
    canaryData[0]            = CANARY_LEFT;
    canaryData[capacity + 1] = CANARY_RIGHT;
}


void stackInit(stack_t *const stk, size_t capacity ON_DEBUG(, debug_info_t debugInfo))
{
    if(!stk)
        return;

    if(capacity > CAPACITY_MAX)
        return;

    stk->canaryUp   = CANARY_STUCT_UP;
    stk->canaryData = (stack_element_t*) calloc(capacity + CANARY_COUNT, sizeof(stack_element_t));
    stk->data       = &stk->canaryData[1];
    stk->capacity   = capacity;

    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
        return;

    canaryAdder(stk->canaryData, stk->capacity);

    stk->size = 0;
    stk->err = noProblem;

    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != 0)
        return;

    if (mainVerifier(stk ON_DEBUG(, debugInfo)) != 0)
    {
        fprintf(stderr, "Critical error. Check debug file\n");
        return;
    }
    stk->canaryDown = CANARY_STUCT_DOWN;
}


void destroyStack(stack_t *const stk ON_DEBUG(, debug_info_t debugInfo))
{
    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
        return;

    free(stk->canaryData);
    stk->data     = nullptr;
    stk->capacity = 0;
    stk->size     = 0;
    stk->err      = noProblem;
    free(stk);
}


void stackPush(stack_t *const stk, stack_element_t value ON_DEBUG(, debug_info_t debugInfo))
{
    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
        return;

    if (mainVerifier(stk ON_DEBUG(, debugInfo)) != 0)
    {
        fprintf(stderr, "Critical error. Check debug file\n");
        return;
    }

    if (canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryLeftDead || canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryRightDead)
        return;


    if(!pushRealloc(stk))
        return;


    stk->data[stk->size++] = value;

    if (mainVerifier(stk ON_DEBUG(, debugInfo)) != 0)
    {
        fprintf(stderr, "Critical error. Check debug file\n");
        return;
    }

    if (canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryLeftDead || canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryRightDead)
        return;
}




void printStackConsole(stack_t *stk ON_DEBUG(, debug_info_t debugInfo))
{
    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
    {
        return;
    }

    if (canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryLeftDead || canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryRightDead)
        return;


    printf(" size = %zu, capacity = %zu, error = %d\n", stk->size, stk->capacity, stk->err);

    printf("Elements:\n");
    for (size_t i = 0; i < stk->size; i++)
    {
        printf("[%zu] = %d\n", i, stk->data[i]);
    }
}




errors canaryChecker(stack_t *const stk ON_DEBUG(, debug_info_t debugInfo))
{
    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
    {
        fprintf(stderr, "NULL problem");

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, stk->err);
    #endif
        return nullProblem;
    }
    if (stk->canaryUp != CANARY_STUCT_UP)
    {
        fprintf(stderr, "Your UP canary dead:(. Check memory\n");
        stk->err = canaryUpDead;

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, canaryUpDead);
    #endif

        return canaryUpDead;
    }

    if (stk->canaryDown != CANARY_STUCT_DOWN)
    {
        fprintf(stderr, "Your DOWN canary dead:(. Check memory\n");
        stk->err = canaryDownDead;

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, canaryDownDead);
    #endif

        return canaryDownDead;
    }

    if (stk->canaryData[0] != (stack_element_t) CANARY_LEFT)
    {
        fprintf(stderr, "Your LEFT canary dead:(. Check memory\n");
        stk->err = canaryLeftDead;

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, canaryLeftDead);
    #endif

        return canaryLeftDead;
    }
    if (stk->canaryData[stk->capacity+1] != (stack_element_t) CANARY_RIGHT)
    {
        fprintf(stderr, "Your RIGHT canary dead:(. Check memory\n");
        stk->err = canaryRightDead;

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, canaryRightDead);
    #endif

        return canaryRightDead;
    }
    return noProblem;
}





errors mainVerifier(stack_t *const stk ON_DEBUG(, debug_info_t debugInfo))
{
    if(stk == NULL)
    {
        fprintf(stderr, "Function called with NULL pointer. Check debug file\n");

    #ifdef STK_DEBUG
        makeDebugLog(NULL, debugInfo, nullProblem);
    #endif

        return nullProblem;
    }

    if (stk->data == NULL)
    {
        stk->err = callocError;
        fprintf(stderr, "Memory error\n");

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, stk->err);
    #endif
        return stk->err;
    }

    stk->err = noProblem;

    if (stk->capacity > CAPACITY_MAX)
    {
        stk->err = capacityProblem;
        fprintf(stderr, "Capacity error\n");

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, stk->err);
    #endif

        return stk->err;
    }


    if (stk->size > stk->capacity || stk->size > CAPACITY_MAX)
    {
        stk->err = capacityProblem;
        fprintf(stderr, "Capacity error\n");

    #ifdef STK_DEBUG
        makeDebugLog(stk, debugInfo, stk->err);
    #endif

        return stk->err;
    }

    return stk->err;
}






stack_element_t stackPop(stack_t *const stk ON_DEBUG(, debug_info_t debugInfo))
{
    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
    {
        return (stack_element_t) POISON_VALUE;
    }

    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != 0)
        return (stack_element_t) POISON_VALUE;

    if (canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryLeftDead || canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryRightDead)
        return (stack_element_t) POISON_VALUE;

    if(stk->size == 0)
    {
        fprintf(stderr, "Void stack\n");
        return (stack_element_t) POISON_VALUE;
    }

    stk->size--;
    stack_element_t returnedData = stk->data[stk->size];
    popRealloc(stk);

    if(mainVerifier(stk ON_DEBUG(, debugInfo)) != noProblem)
    {
        return (stack_element_t) POISON_VALUE;
    }

    if (canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryLeftDead || canaryChecker(stk ON_DEBUG(, debugInfo)) == canaryRightDead)
        return (stack_element_t) POISON_VALUE;

    return returnedData;
}



bool popRealloc(stack_t *const stk)
{
    if (stk->capacity > 4 && stk->size == stk->capacity/4)
    {
        size_t newCapacity = (stk->capacity / 4);


        stack_element_t* temp = (stack_element_t*) realloc(stk->canaryData, (newCapacity + CANARY_COUNT) * sizeof(stack_element_t));

        if(temp == NULL)
        {
            fprintf(stderr, "Error with memory getting\n");
            return false;
        }

        canaryAdder(temp, newCapacity);

        stk->canaryData  = temp;
        stk->data = &stk->canaryData[1];
        stk->capacity = newCapacity;
    }
    return true;
}


bool pushRealloc(stack_t *const stk)
{
    if (stk->size == stk->capacity)
    {
        size_t           newCapacity = stk->capacity * 2;
        stack_element_t *temp        = (stack_element_t*) realloc(stk->canaryData, (newCapacity + CANARY_COUNT) * sizeof(stack_element_t));

        if (temp == NULL)
        {
            printf("Memory getting problem\n");
            return false;
        }

        temp[stk->capacity + 1] = POISON_VALUE;
        for (size_t i = stk->capacity; i < newCapacity; i++)
        {
            temp[i + 1] = POISON_VALUE;
        }
        canaryAdder(temp, newCapacity);

        stk->canaryData = temp;
        stk->data       = &stk->canaryData[1];
        stk->capacity   = newCapacity;
    }
    return true;
}



bool changeMemoryUpStack(stack_t *const stk, size_t newCapacity)
{
    if(newCapacity < stk->capacity)
        return false;

    stack_element_t *temp = (stack_element_t*) realloc (stk->canaryData, (newCapacity +CANARY_COUNT )*sizeof(stack_element_t));

    if (temp == NULL)
        return false;

    temp[stk->capacity + 1] = POISON_VALUE;
    for (size_t i = stk->capacity; i < newCapacity; i++)
    {
        temp[i + 1] = POISON_VALUE;
    }

    canaryAdder(temp, newCapacity);

    canaryAdder(temp, newCapacity);
    stk->canaryData = temp;
    stk->data = &stk->canaryData[1];
    stk->capacity = newCapacity;
    return true;
}

const char* getErrorName(errors err)
{
    switch(err)
    {
        case noProblem:
            return "no problem";
        case callocError:
            return "calloc problem";
        case capacityProblem:
            return "capacity problem";
        case sizeProblem:
            return "size problem";
        case alreadyInit:
            return "already init problem";
        case initProblem:
            return "initialize problem";
        case nullProblem:
            return "null pointer problem";
        case canaryLeftDead:
            return "left canary problem";
        case canaryRightDead:
            return "right canary problem";
        case canaryUpDead:
            return "upper canary in stack problem";
        case canaryDownDead:
            return "downer canary in stack problem";
        default:
            return "undefined problem";

    }
}
