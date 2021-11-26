#include "./utils/scanUtils.h"
#include "./utils/scanUtilsProps.h"

#include "first_pass.h"

struct NodeList *externals;
struct dataNode *dataList;
struct memNode *MemList;
struct symbolNode *symbolTable;

int ICF = 0;
int DCF = 0;

typedef enum {
    false,
    true
} boolean;


char Data_Insts[4][10] = {
    ".dh\0",
    ".dw\0",
    ".db\0",
    ".asciz\0",
};

char *isDataInstruction(char *str){
    int i;

    for(i=0; i < sizeof(Data_Insts)/sizeof(Data_Insts[0]); i++){
        if(strstr(str, Data_Insts[i])){
            return Data_Insts[i];
        }
    }

    return NULL;
}

int firstScan(FILE *fptr, char *filename){
    
    char *currCmd, *operand;
    char input[LINE_SIZE];

    int lineNum = 0;
    int commentIndicatorIndex = -1;
    int error_code = 0;
    int dataType;

    int IC = 100;
    int DC = 0;
    int num,i;


    char *currSymbol;
    char *currDataInst;

    struct dataNode  *ptrDataList;
    struct memNode *ptrMemList;

    char *commandName;
    int currCommandType;

    externals = NULL;
    MemList = NULL;
    symbolTable = NULL;
    dataList = NULL;

    memset(input, '\0', LINE_SIZE);

    loop:
    while(fgets(input, LINE_SIZE, fptr) != NULL){
        boolean isSymbolDefinition = false;
        if(strlen(strip(input)) == 0)
            goto loop;
        if(validateLine(input, ++lineNum, filename)){
            error_code = 1;
            goto loop;
        }
        
        /* get index of comment, if there is, cut the comment out of the string */
        commentIndicatorIndex = getCommentIndicatorIndex(input);
        if(commentIndicatorIndex > 0){
            input[commentIndicatorIndex] = '\0';
        }else if(commentIndicatorIndex == 0){
            /* checking if 0 individually because if a line */
            /* is blank, the assembler skips the line. */
            goto loop;
        }

        currCmd = strip(input); /* the current line without whitespaces */
        /* check if the first field in line is a symbol, if it is a symbol we need to later on add it properly to the symbol table */
        if((currSymbol = isSymbolDef(currCmd)) != NULL){
            isSymbolDefinition = true;   
        }

        if((currDataInst = isDataInstruction(currCmd)) != NULL){ 
            /*it is a data instruction*/
            /*if the first field was symbol we will add it to the symbol table */
            /* with the flag isData=1 because we are in data instruction */
            if(isSymbolDefinition){
                addSymbol(currSymbol, DC, 0,0,0,1);
                currCmd += (strlen(currSymbol) + 1); /* the ':' of the symbol is also a char we want to get rid of*/
            }
            
            currCmd += strlen(currDataInst);
            if(!strcmp(currDataInst, ".asciz") ){
                char *ptr = input;

                /* parsing the asciz string out of the command */
                while(*ptr != '"')
                    ptr++;

                ptr += 1;
                dataType = 1;
                while(*ptr != '"'){
                    if(dataList == NULL){
                        dataList = malloc(sizeof(struct dataNode));
                        dataList->address = DC;
                        dataList->dataCommand.dbCommand.data = (int)(*ptr);
                        dataList->dataType = dataType;
                        ptrDataList = dataList;
                    }else{
                        addData(ptrDataList, DC, (int) (*ptr), dataType);
                        ptrDataList = ptrDataList->next;
                    }
                    ptr ++;
                    DC ++;
                }
                /* we also need to add the character - '\0' */
                if(dataList == NULL){
                    dataList = malloc(sizeof(struct dataNode));
                    dataList->address = DC;
                    dataList->dataCommand.dbCommand.data = (int)('\0');
                    dataList->dataType = dataType;
                    ptrDataList = dataList;
                }else{
                    addData(ptrDataList, DC, (int) ('\0'), dataType);
                    ptrDataList = ptrDataList->next;
                }
                DC ++;
            }else {

                if (!strcmp(currDataInst, ".db")) {
                    dataType = 1;
                } else if (!strcmp(currDataInst, ".dh")) {
                    dataType = 2;
                } else if (!strcmp(currDataInst, ".dw")) {
                    dataType = 4;
                }
                operand = (char *) calloc(strlen(currCmd)+1, sizeof(char));
                for (i = 0; i < strlen(currCmd) + 1; i++) {
                    if ((currCmd[i] == ',') ||(currCmd[i] == '\0')) {
                        num = atoi(operand);
                        if (dataList == NULL) {
                            dataList =  malloc(sizeof(struct dataNode));
                            dataList->address = DC;
                            dataList->dataType = dataType;
                            if (dataType == DB) {
                                dataList->dataCommand.dbCommand.data = num;
                            } else if (dataType == DH) {
                                dataList->dataCommand.dhCommand.data = num;
                            } else {
                                dataList->dataCommand.dwCommand.data = num;
                            }
                            ptrDataList = dataList;
                        } else {
                            addData(ptrDataList, DC, num, dataType);
                            ptrDataList = ptrDataList->next;
                        }

                        DC += dataType;

                        operand[0] = '\0';
                    } else {
                        char sting_char[2];
                        sting_char[0] =  currCmd[i];
                        sting_char[1]= '\0';
                        strcat(operand, sting_char);
                    }
                }
            }
            goto loop;
        }else{


            if(strstr(currCmd, ".extern") || strstr(currCmd, ".entry")){

                /* if the command is is an extern command and we should add its operand to the symbol table */
                /* otherwise it is an entry command it will be handled in the second pass*/
                if(strstr(currCmd, ".extern")){
                    currSymbol = currCmd + strlen(".extern");
                    if(getSymbol(currSymbol)!=NULL){
                        if(!getSymbol(currSymbol)->isExternal){
                            fprintf(stdout, "(%s) In line %d: the symbol %s cannot be both external and not external\n", filename, lineNum, currSymbol);
                            error_code = 1;
                        }
                    }else{
                        /*the operand to the .extern command is symbol we will add it to the symbol table with the flag isExternal=1 */
                        addSymbol(currSymbol, 0, 1, 0, 0, 0);
                    }
                }

                goto loop;
            }

            /* it is a directive command*/
            /* if the first field was symbol we will add it to the symbol table */
            /* with the flag isCode=1 because it is a directive command */
            if(isSymbolDefinition){
                addSymbol(currSymbol,IC,0, 0, 1, 0);
                currCmd += (strlen(currSymbol) + 1);
            }
            commandName = firstWord(input);

            /* find the current command type */
            currCommandType = getCommandType(commandName);
            switch(currCommandType){
                case R_Type:
                    if(MemList == NULL) {
                        MemList = malloc(sizeof(struct memNode));
                        MemList->address = IC;
                        MemList->commandType = R_COMMAND;
                        MemList->command.commandR = handleRCommand(currCmd);
                        ptrMemList = MemList;
                    } else{
                        addCommandR(ptrMemList, IC, handleRCommand(currCmd));
                        ptrMemList = ptrMemList->next;
                    }
                    break;
                
                case I_Type:
                    if(MemList == NULL) {
                        MemList = malloc(sizeof(struct memNode));
                        MemList->address = IC;
                        MemList->commandType = I_COMMAND;
                        MemList->command.commandI = handleICommand(currCmd);
                        ptrMemList = MemList;
                    } else{
                        addCommandI(ptrMemList, IC, handleICommand(currCmd));
                        ptrMemList = ptrMemList->next;
                    }
                    break;
                
                case J_Type:
                    if(MemList == NULL) {
                        MemList = malloc(sizeof(struct memNode));
                        MemList->address = IC;
                        MemList->commandType = J_COMMAND;
                        MemList->command.commandJ = handleJCommand(currCmd);
                        ptrMemList = MemList;
                    } else{
                        addCommandJ(ptrMemList, IC, handleJCommand(currCmd));
                        ptrMemList = ptrMemList->next;
                    }
                    break;

                default:
                    break;
            }

        }
        IC += 4;
    }

    ICF = IC;
    DCF = DC;

    ptrMemList = MemList;
    if(dataList != NULL){
        ptrDataList->next = NULL;
        for(ptrDataList = dataList; ptrDataList != NULL; ptrDataList = ptrDataList->next){
            ptrDataList->address = ptrDataList->address + ICF;
        }
    }
    if(symbolTable != NULL){

        struct symbolNode *curr = symbolTable;
        while(curr != NULL){
            if(curr->isData){
                curr->value += ICF;
            }
            
            curr = curr->next;
        }
    }
    return error_code;
}
