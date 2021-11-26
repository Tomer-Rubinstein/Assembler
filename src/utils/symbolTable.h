extern struct symbolNode *symbolTable;

struct symbolNode{
    char *symbol;
    int value;
    int isExternal;
    int isEntry;
    int isCode;
    int isData;
    struct symbolNode *next;
};

void addSymbol(char *symbol, int value,int isExternal, int isEntry, int isCode, int isData);
struct symbolNode *getSymbol(char *symbol);
void freeSymbolTable();
