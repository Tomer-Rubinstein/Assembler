#define DATA_INSTRUCTION_COUNT 6
#define KEYWORDS_COUNT 33
#include <limits.h>


char keywords[KEYWORDS_COUNT][8] = {
    "add\0",
    "sub\0",
    "and\0",
    "or\0",
    "nor\0",
    "move\0",
    "mvhi\0",
    "mvlo\0",
    "addi\0",
    "subi\0",
    "andi\0",
    "ori\0",
    "nori\0",
    "bne\0",
    "beq\0",
    "blt\0",
    "bgt\0",
    "lb\0",
    "sb\0",
    "lw\0",
    "sw\0",
    "lh\0",
    "sh\0",
    "jmp\0",
    "la\0",
    "call\0",
    "stop\0",
    ".dh\0",
    ".dw\0",
    ".db\0",
    ".asciz\0",
    ".entry\0",
    ".extern\0",
};

char Data_Instructions[DATA_INSTRUCTION_COUNT][10] = {
    ".dh\0",
    ".dw\0",
    ".db\0",
    ".asciz\0",
};

long DataInstructionRangeValues[3][2] = {
    {-32768, 32767},
    {INT_MIN, INT_MAX},
    {-128, 127}
};
