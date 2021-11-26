#include "MemNode.h"
#include <stdio.h>
#include <stdlib.h>



/*
    addCommandR() adds a new node representing a command type R to the linked list
    params:
        - struct memNode* prev_node
        - int ic, the current address of the symbol
        - commandR commandR1 the command to be added
    returns:
        void
*/
void addCommandR(struct memNode* prev_node, int ic, commandR commandR1){
    if (prev_node != NULL) {
        struct memNode *new_node = calloc(1, sizeof(struct memNode));
        new_node->address = ic;
        new_node->commandType = R_COMMAND;
        new_node->command.commandR = commandR1;
        prev_node->next = new_node;
    }
}

/*
    addCommandR() adds a new node representing a command type R to the linked list
    params:
        - struct memNode* prev_node
        - int ic, the current address of the symbol
        - commandI commandI1 the command to be added
    returns:
        void
*/
void addCommandI(struct memNode* prev_node, int ic, commandI commandI1){
    if (prev_node != NULL) {
        struct memNode *new_node =  calloc(1, sizeof(struct memNode));
        new_node->address = ic;
        new_node->commandType = I_COMMAND;
        new_node->command.commandI = commandI1;
        prev_node->next = new_node;
    }
}

/*
    addCommandJ() adds a new node representing a command type R to the linked list
    params:
        - struct memNode* prev_node
        - int ic, the current address of the symbol
        - commandJ commandJ1 the command to be added
    returns:
        void
*/
void addCommandJ(struct memNode* prev_node, int ic, commandJ commandJ1){
    if (prev_node != NULL) {
        struct memNode *new_node = calloc(1, sizeof(struct memNode));
        new_node->address = ic;
        new_node->commandType = J_COMMAND;
        new_node->command.commandJ = commandJ1;
        prev_node->next = new_node;
    }
}


void freeMemList(){
    struct memNode* tmp;

    while (MemList != NULL){
        tmp = MemList;
        MemList = MemList->next;
        free(tmp);
    }
}
