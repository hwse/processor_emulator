#include<stdlib.h>
#include<stdio.h>

#include"processor.h"

int main(int argc, char const *argv[])
{
    char ops[] = {MV_CONST_A, 255, DEC_A, JNZ, 2, 0, HLT};
    processor* proc = init_processor(ops);
    print_state(proc);
    run(proc);
    print_state(proc);
    free(proc);
    return 0;
}
