#include<stdlib.h>
#include<stdio.h>

#include"processor.h"

int main(int argc, char const *argv[])
{
    char ops[] = {MV_CONST_A, 200, MV_CONST_D, 200, ADD_D, MV_CONST_B, 1, DEC_B, HLT};
    processor* proc = init_processor(ops);
    print_state(proc);
    run(proc);
    print_state(proc);
    free(proc);
    return 0;
}
