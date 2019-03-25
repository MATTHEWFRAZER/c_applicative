#include "applicative.h"
#include <stdio.h>
#include <malloc.h>

void* just_a_test(void** args, int length)
{
    int i = 0;
    for(; i<length; ++i)
    {
        printf("printed %d\n", *((int*)args[i]));
    }
    return NULL;
}


int main(int argc, char* argv[])
{
    AF* af  = pure_from_arrow(just_a_test, 3);
    int i = 2;
    int j = 3;
    int h = 4;
    AF* af2 = pure(&i);
    AF* af3 = pure(&j);
    AF* af4 = pure(&h);
    af = apply(af, af2);
    af = apply(af, af3);
    af = apply(af, af4);

    free_applicative_functor(af);
	free_applicative_functor(af2);
    free_applicative_functor(af3);
    free_applicative_functor(af4);
    return 0;
}