#include <stdio.h>
#include <stdlib.h>

#include "../aria.h"

static ar_Value *call_me(ar_State* S, ar_Value* args, ar_Value *env) {
    printf("I'm called from the custom library!\n");
    return ar_new_number(S, 42);
}

void init_lib(ar_State *S) {
    int i;
    struct { const char *name; ar_Prim fn; } prims[] = {
        { "callme", call_me },
        { NULL, NULL }
    };
    for (i = 0; prims[i].name; i++) {
        ar_bind_global(S, prims[i].name, ar_new_prim(S, prims[i].fn));
    }
}
