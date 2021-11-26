extern struct NodeList *externals;

struct NodeList{
    int address;
    char *label;
    struct NodeList *next;
};

void addExternal(struct NodeList* prev_node, int ic, char * encoded);
void freeExternals();
void printExternals(struct NodeList *head);
