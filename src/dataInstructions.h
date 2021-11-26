#define DATA_INSTRUCTION_COUNT 6

char Data_Instructions[DATA_INSTRUCTION_COUNT][10] = {
    ".dh\0",
    ".dw\0",
    ".db\0",
    ".asciz\0",
    ".entry\0",
    ".extern\0",
};


int DataInstructionRangeValues[3][2] = {
    {-32768, 32767},
    {-2147483648, 2147483647},
    {-128, 127}
};
