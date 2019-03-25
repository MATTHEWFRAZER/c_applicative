#ifndef APPLICATIVE_H
#define APPLICATIVE_H

#include <string.h>

typedef union ic
{
    void* inner_context;
    void* (*inner_context_arrow)(void**, int length);
}IC;

typedef struct af
{
    IC*    inner_context;
    int    expected_args;
    int    applied_args;
    void** args;
}AF;

AF* pure(void* inner_context);

AF* pure_from_arrow(void* (*arrow)(void** args, int length), int expected_args);

AF* apply(AF* lifted_arrow, AF* apply_to);

void free_applicative_functor(AF* af);
#endif