#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#define END_OF_LIST 2


int nodeCounter=0;

/*
    addSymbol() adds a new node representing a symbol to the linked list
    params:
        - char *symbol, the symbol name
        - int value, the current address of the symbol
        - int isExternal (boolean)
        - int isEntry (boolean)
        - int isCode (boolean)
        - int isData (boolean)
    returns:
        void
*/
void addSymbol(char *symbol, int value, int isExternal, int isEntry, int isCode, int isData){
  struct symbolNode *curr = calloc(1, sizeof(struct symbolNode));

    if(symbolTable == NULL){
        symbolTable = calloc(1, sizeof(struct symbolNode));
        symbolTable->symbol = (char *) malloc(sizeof(char) * 33);

        memset(symbolTable->symbol, '\0', strlen(symbol));
        strcat(symbolTable->symbol, symbol);

        symbolTable->value = value;
        symbolTable->isExternal = isExternal;
        symbolTable->isEntry = isEntry;
        symbolTable->isCode = isCode;
        symbolTable->isData = isData;

		free(curr);
        return;
    }


    curr->symbol = (char *) malloc(sizeof(char) * 33);

    memset(curr->symbol, '\0', strlen(symbol));
	strcat(curr->symbol, symbol);

    curr->value = value;
    curr->isExternal = isExternal;
    curr->isEntry = isEntry;
    curr->isCode = isCode;
    curr->isData = isData;

    curr->next = symbolTable->next;
    symbolTable->next = curr;
	return;
}

/* 
    getSymbol() returns the symbol with the same name as the paramater from the symbol table linked list
    params:
        char *symbol, which symbol to look for
    returns:
        (struct symbolNode *) a pointer to that symbol with the same name as symbol(param)
*/
struct symbolNode *getSymbol(char *symbol){

    struct symbolNode *curr = symbolTable;
    while(curr != NULL){
        if(!strcmp(curr->symbol, symbol)){
            return curr;
        }
        curr=curr->next;
    }
    return NULL;
}

void freeSymbolTable(){
    struct symbolNode* tmp;

    while (symbolTable != NULL){
        tmp = symbolTable;
        symbolTable = symbolTable->next;
        free(tmp->symbol);
        free(tmp);
    }
}

