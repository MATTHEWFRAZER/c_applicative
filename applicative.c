#include "applicative.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

void apply_argument(AF* lifted_arrow, AF* apply_to, int new_applied_length)
{
    lifted_arrow->args[lifted_arrow->applied_args] = apply_to->inner_context->inner_context;
    lifted_arrow->applied_args = new_applied_length;
}

AF* apply_arguments(AF* lifted_arrow, AF* apply_to)
{
    // we are applying args but not enough so we're not ready to do computation
    int new_applied_length = lifted_arrow->applied_args + 1;
    if(lifted_arrow->expected_args > new_applied_length)
    {
       apply_argument(lifted_arrow, apply_to, new_applied_length);
       return lifted_arrow;
    }
    // we have applied the right amount of arguments to inner arrow
    else if(lifted_arrow->expected_args == new_applied_length)
    {
       apply_argument(lifted_arrow, apply_to, new_applied_length);
       void* result = lifted_arrow->inner_context->inner_context_arrow(lifted_arrow->args, lifted_arrow->expected_args);       lifted_arrow->inner_context->inner_context = result;
       return lifted_arrow;
    }
    // we have already done what ever computation we needed, we can no longer apply more stuff
	else
    {
      return NULL;
    }
}

AF* new_applicative_functor(int expected_args)
{
    AF* af = malloc(sizeof(AF));
    IC* ic = malloc(sizeof(IC));
    af->inner_context = ic;
    af->expected_args = expected_args;
    af->args = malloc(sizeof(void*)*expected_args);
    af->applied_args = 0;
    return af;
}

void free_applicative_functor(AF* af)
{
    if(af->inner_context != NULL)
    {
        free(af->inner_context);
    }
    if(af->args != NULL)
    {
        free(af->args);
    }
    free(af);
}

AF* pure(void* inner_context)
{
    AF* af = new_applicative_functor(0);
    af->inner_context->inner_context = inner_context;
    return af;
}

//<$> [(a->b) -> A[(a->b)]]
AF* pure_from_arrow(void* (*arrow)(void** args, int length), int expected_args)
{
   AF* af = new_applicative_functor(expected_args);
   af->inner_context->inner_context_arrow = arrow;
   return af;
}

//<*> [A[(a->b)]] -> A[a] -> A[b]
AF* apply(AF* lifted_arrow, AF* apply_to)
{
   return apply_arguments(lifted_arrow, apply_to);
}