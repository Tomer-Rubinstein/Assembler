#include <stdio.h>
#include <stdlib.h>
#include "dataList.h"

struct dataNode *dataList;

void addData(struct dataNode* prev_node, int dc,int num ,int dataType){
    if (prev_node != NULL) {
        struct dataNode *new_node = malloc(sizeof(struct dataNode));
        new_node->address = dc;
        new_node->dataType = dataType;
        if(dataType== DB){
            new_node->dataCommand.dbCommand.data = num;
        }else if(dataType== DH){
            new_node->dataCommand.dhCommand.data = num;
        }else{
            new_node->dataCommand.dwCommand.data = num;
        }
        prev_node->next = new_node;
    }
}

void freeDataList(){
    struct dataNode* tmp;

    while (dataList != NULL){
       tmp = dataList;
       dataList = dataList->next;
       free(tmp);
    }
}
