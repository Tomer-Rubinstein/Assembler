#include "./utils/symbolTable.h"
#include "dataList.h"
#include "MemNode.h"
#include "LabelNode.h"

void outputObjectFile(struct memNode *, struct dataNode *, char *);
void outputExternFile(struct NodeList *, char *);
void outputEntryFile(struct symbolNode *, char *);