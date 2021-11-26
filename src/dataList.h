#define DB_BITFIELD_SIZE 8
#define DH_BITFIELD_SIZE 16
#define DW_BITFIELD_SIZE 32

extern struct dataNode *dataList;

typedef struct db{
  unsigned int data:DB_BITFIELD_SIZE;
} db;

typedef struct dh{
  unsigned int data:DH_BITFIELD_SIZE;
} dh;

typedef struct dw{
  unsigned int data:DW_BITFIELD_SIZE;
} dw;

struct dataNode{
  enum{DB=1, DH=2, DW=4} dataType;
  int address;
  struct dataNode *next;
  union dataCommand{
    db dbCommand;
    dh dhCommand;
    dw dwCommand;
  } dataCommand;
};

void addData(struct dataNode* prev_node, int dc,int num ,int dataType);
void freeDataList();
