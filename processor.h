#include<stdint.h>
#include<stdbool.h>

struct processor {
    uint8_t reg_a, reg_b, reg_c, reg_d; // registers with 8 byte content
    uint8_t* prog_counter;              // address of current op 
    bool zero_flag, carry_flag;        // flags modified by some instructions 
    bool on;                            // is processor on
};
typedef struct processor processor;

typedef uint8_t operation;
#define HLT 0 //stop the processor
#define NOP 1 //do nothing

// --- MOVE INSTRUCTIONS ---
#define MV_CONST_A 16 // constant move
#define MV_CONST_B 17
#define MV_CONST_C 18
#define MV_CONST_D 19
#define MV_A_B 20 // from a move
#define MV_A_C 21 
#define MV_A_D 22 
#define MV_B_A 23 //from b move
#define MV_B_C 24
#define MV_B_D 25
#define MV_C_A 26 //from c move
#define MV_C_B 27
#define MV_C_D 28
#define MV_D_A 29 //from d move
#define MV_D_B 30
#define MV_D_C 31 
// TODO: memory access

// artithmetic operations
#define ADD_A 64 // add regsiter to a
#define ADD_B 65 
#define ADD_C 66
#define ADD_D 67
#define SUB_A 68 // sub regsiter from a
#define SUB_B 69 
#define SUB_C 70
#define SUB_D 71
#define MUL_A 72 // mul regsiter with a
#define MUL_B 73 
#define MUL_C 74
#define MUL_D 75
#define DIV_A 76 // divide a by register and store remainder in register 
#define DIV_B 77 
#define DIV_C 78
#define DIV_D 79
#define INC_A 80 //increase register by one
#define INC_B 81
#define INC_C 82
#define INC_D 83
#define DEC_A 84 //decrease register by one
#define DEC_B 85
#define DEC_C 86
#define DEC_D 88

processor* init_processor(uint8_t* memory);
void run(processor* proc);

void print_state(processor* proc);
void execute_opertion(processor* proc);

