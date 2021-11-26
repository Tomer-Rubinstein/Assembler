#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LabelNode.h"



void addExternal(struct NodeList* prev_node, int ic, char * label){
    if (prev_node != NULL) {
        struct NodeList *new_node = calloc(1, sizeof(struct NodeList));
        new_node->address = ic;
        new_node->label = (char *) malloc(sizeof(char) * 32);
		memset(new_node->label, '\0', 32);
        strcat(new_node->label, label);
        prev_node->next = new_node;
    }
}

void freeExternals(){
    struct NodeList* tmp;

    while (externals != NULL){
       tmp = externals;
       externals = externals->next;
       free(tmp->label);
       free(tmp);
    }
}
