extern struct memNode *MemList;

typedef struct commandR{
    unsigned int opcode: 6;
    unsigned int rs: 5;
    unsigned int rt: 5;
    unsigned int rd: 5;
    unsigned int funct: 5;
    unsigned int unused: 6;
} commandR;

typedef struct commandI{
    unsigned int opcode: 6;
    unsigned int rs: 5;
    unsigned int rt: 5;
    unsigned int immed: 16;
} commandI;

typedef struct commandJ{
    unsigned int opcode: 6;
    unsigned int reg: 1;
    unsigned int address: 25;
} commandJ;

struct memNode{
    enum{R_COMMAND, I_COMMAND, J_COMMAND} commandType;
    int address;
    struct memNode *next;
    union commands{
        commandR commandR;
        commandI commandI;
        commandJ commandJ;
    } command;
};


void addCommandR(struct memNode* prev_node, int ic, commandR commandR1);
void addCommandI(struct memNode* prev_node, int ic, commandI commandI1);
void addCommandJ(struct memNode* prev_node, int ic, commandJ commandJ1);
void freeMemList();
void printMemList();
